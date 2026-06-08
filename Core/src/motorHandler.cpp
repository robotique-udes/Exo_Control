#include "motorHandler.h"

MotorHandler::MotorHandler()
{
    stateMutex = xSemaphoreCreateMutex();

    motors[motor_config::hip_right] = &hipRightMotor;
    motors[motor_config::hip_left] = &hipLeftMotor;
    motors[motor_config::knee_right] = &kneeRightMotor; 
    motors[motor_config::knee_left] = &kneeLeftMotor;
}

MotorHandler::~MotorHandler()
{
    exitMotors();
}

void MotorHandler::update(const float torque[motor_config::amount])
{
/*     if (tempTooHigh == true)
    {
        //slowShutDown();
        //TODO remplacer ca par le slow shutdown
        float shutDown[NB_MOTORS] = {0,0,0,0};
        applyTorque(shutDown);
        return;
    } */

    applyTorque(torque);
}

void MotorHandler::initializeMotors()
{
    for (int motorPos = 0; motorPos < motor_config::amount; motorPos++)
    {
        motors[motorPos]->enterMode();
    }

    initialized = true;
}

void MotorHandler::exitMotors()
{
    for (int motorPos = 0; motorPos < motor_config::amount; motorPos++)
        //motors[motorPos]->stop();

    initialized = false;
}

void MotorHandler::applyTorque(const float torque[motor_config::amount])
{

    //TODO wtf is this
    //float newTorque[4] = {torque[0], torque[2], torque[1], torque[3]};
    //float newTorque[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    for (int motorIndex = 0; motorIndex < motor_config::amount ; motorIndex++)
    {
        float motorTorque = 0.0f;
        if(initialized)
        {
            for(int sampleIndex = 0; sampleIndex < (motor_config::moving_avg_size - 1); ++sampleIndex)
            {
                movingAverage[motorIndex][sampleIndex] = movingAverage[motorIndex][sampleIndex + 1];
                motorTorque += movingAverage[motorIndex][sampleIndex];
            }
            
            //to keep the moving average smooth
            // TODO moving avg should be in the motor class
            movingAverage[motorIndex][motor_config::moving_avg_size - 1] = torque[motorIndex]*motorOn;
            motorTorque += movingAverage[motorIndex][motor_config::moving_avg_size - 1];

            motorTorque /= motor_config::moving_avg_size;
        }
        if (motorIndex == motor_config::hip_left
            || motorIndex == motor_config::knee_left)
        {
            motorTorque = -1*motorTorque;
        }  

        //send torque request to the respective motor
        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) 
        {
            if (debug::motor_handler)
            {
                Serial.print("Motor : ");
                Serial.print(motorIndex);
                Serial.print(" id : ");
                Serial.print(motors[motorIndex]->getMotorID());
                Serial.print(" | Torque : ");
                Serial.println(motorTorque);
            }
           // motors[motorIndex]->sendRequest(TORQUE, motorTorque*motorOn);
            motors[motorIndex]->sendCommand(0.0f, 0.0f, motorTorque, 0.0f, 0.0f);
            xSemaphoreGive(stateMutex);
        }
        initialShutdownTorque[motorIndex] = motorTorque;
        readCanReplyBuffer();

        //checks if the respective motor is overheating
        float temperature = motors[motorIndex]->getTemperature();

        if (debug::motor_handler)
        {
            Serial.print("Temperature : ");
            Serial.println(temperature);
        }
        continue;
        if (temperature > motor_config::max_temperature)
        {
            Serial.println(temperature);
            tempTooHigh = true;
            shutdownStartTime = millis();
            return;
        }
    }
    Serial.println();
}

//TODO tester le slow shutdown
void MotorHandler::slowShutDown()
{
    unsigned long timeElapsed = millis() - shutdownStartTime;

    //if the timer is done, shuts down all the motors
    if (timeElapsed > motor_config::shut_down_time)
    {  
        tempTooHigh = false;
        exitMotors();
        return;
    }

    //if not, linearly reduces the torque until the timer ends
    for (int motorIndex = 0; motorIndex < motor_config::amount; motorIndex++)
    {  
        float torque = initialShutdownTorque[motorIndex] 
            * (1 - ((float)timeElapsed / motor_config::shut_down_time));
        motors[motorIndex]->sendCommand(0.0f, 0.0f, torque, 0.0f, 0.0f);
    }
}



void MotorHandler::setMotorState(bool state)
{

    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) 
    {    
        for (int motorIndex = 0; motorIndex < motor_config::amount ; motorIndex++)
        {
            //motors[motorIndex]->setMotorState(state);
        }
        this->motorOn = state;
        xSemaphoreGive(stateMutex);
    }
}



void MotorHandler::readCanReplyBuffer()
{
    while(ESP32Can.readFrame(&msg, 0))
    {
        uint8_t source_id = msg.identifier;
        if (source_id >= 0 && source_id <= motor_config::amount) {
            motors[source_id]->receiveCommand(msg);
        }
    }
}
