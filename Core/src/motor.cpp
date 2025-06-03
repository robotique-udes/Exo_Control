#include "motor.h"

//bool motorMode = ON; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test

Motor::Motor(){}

Motor::Motor(int id)
{
  motorId = id;
}

Motor::~Motor()
{
}

void Motor::sendCanMessage(CanFrame* msg){
  ESP32Can.writeFrame(msg);
}

void Motor::receiveCanMessage(CanFrame* msg){
  ESP32Can.readFrame(msg);
}

//
int Motor::getTemperature(){
  return temperature;
}

//
void Motor::setMotorId(int id){
  motorId = id;
}

//
int Motor::getMotorId(){
  return motorId;
}

enum EnumMotorPosition Motor::getMotorPosition()
{
  return motorPosition;
}

float Motor::getCurrentTorque()
{
  return currentTorque;
}

void Motor::setMotorCorrection(float slope, float offset)
{
  motorCorrectionSlope = slope;
  motorCorrectionOffset = offset;
}

float Motor::getMotorCorrectionSlope()
{
  return motorCorrectionSlope;
}

float Motor::getMotorCorrectionOffset()
{
  return motorCorrectionOffset;
}

unsigned int Motor::float_to_uint(float x, float x_min, float x_max, int bits){
    ///Converts a  float to an unsigned int, given range and number of bits///
    float span = x_max-x_min;
    float offset = x_min;
    unsigned int pgg = 0;
    if(bits==12){
        pgg = (unsigned int) ((x-offset)*4095.0/span);
    }
    if(bits==16){
        pgg = (unsigned int) ((x-offset)*65535.0/span);
    }
    return pgg;
}

float Motor::uint_to_float(unsigned int x_int, float x_min, float x_max, int bits){
    ///converts unsigned int to float, given range and number of bits///
    float span = x_max-x_min;
    float offset = x_min;
    float pgg = 0;
    if (bits==12){
        pgg = ((float) x_int)*span/4095 + offset;
    }
    if (bits==16){
        pgg = ((float) x_int)*span/65535.0 + offset;
    }
    return pgg;
}