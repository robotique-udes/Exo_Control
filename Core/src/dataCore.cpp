#include "dataCore.h"

DataCore *DataCore::instance;

DataCore::DataCore(){
    
    groundDetectEnable = true;
    brightness = HIGH;
    height = 180;
    initialise();
    bnoData = {nullptr, nullptr, nullptr, nullptr, nullptr};
}

void DataCore::initialise(){
    rightGrounded = false;
    leftGrounded = false;
    clutchEnabled = OFF;
    motorEnabled = false;
    motorPower = 2048;
    angleSource = FROM_IMU;
    resetEncoder();
}


DataCore& DataCore::getInstance(){
    if(instance == NULL){
        instance = new DataCore();
    }
    return *instance;
}

bool DataCore::isMotorEnabled(){
    return motorEnabled;
}

bool DataCore::isClutchEnabled(){
    return clutchEnabled;
}

bool DataCore::isGroundDetectEnable(){
    return groundDetectEnable;
}

bool DataCore::isEncoderResetNeeded(){
    return needResetEncoder;
}

//TODO when hmi is refactored, change toggle to using input param
void DataCore::setMotorEnabled(bool setMotorEnabled){
    motorEnabled = setMotorEnabled;
    Serial.print("Motor state: ");
    Serial.println(isMotorEnabled());
}

void DataCore::setClutchEnabled(bool setClutchEnabled){
    clutchEnabled = !(clutchEnabled);

}

void DataCore::setGroundDetectEnable(bool setGroundDetectEnable){
    groundDetectEnable = !(groundDetectEnable);
    Serial.print("\tGround detection set to: ");
    Serial.println(groundDetectEnable);
}

bool DataCore::getAngleSource(){
    return angleSource;
}

void DataCore::setAngleSource(bool setAngleSource){
    angleSource = (setAngleSource);
    Serial.print("\t Angle source set to: ");
    Serial.println(angleSource);
}

bool DataCore::getRightGrounded(){
    return rightGrounded;
}

void DataCore::setRightGrounded(bool state){
    rightGrounded = state;
}

bool DataCore::getLeftGrounded(){
    return leftGrounded;
}

void DataCore::setLeftGrounded(bool state){
    leftGrounded = state;
}

//MOTOR POWER
int DataCore::getMotorPower(){
    return motorPower;
}

void DataCore::setMotorPower(int newMotorPower){
    motorPower = float(newMotorPower) / 100.0 * 4096.0;
    Serial.print("\t Motor power set to: ");
    Serial.println(motorPower);
}

void DataCore::adjustMotorPower(int offset){
    setMotorPower(motorPower+offset);
}

//ENCODER
float DataCore::getEncoderDeg(EnumMotorPosition motor)
{
    long pulses;
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        pulses = encoder_hip_right;
        break;
    case EnumMotorPosition::HIP_L:
        pulses = -encoder_hip_left;
        break;
    case EnumMotorPosition::KNEE_R:
        pulses = -encoder_knee_right;
        break;
    case EnumMotorPosition::KNEE_L:
        pulses = encoder_knee_left;
        break;
    default:
        Serial.println("Invalid motor");
        return 0.0;
    }
    return (float)pulses / PULSES_PER_REVOLUTION * 360.0;
}

float DataCore::getEncoderRad(EnumMotorPosition motor)
{
    long pulses;
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        pulses = encoder_hip_right;
        break;
    case EnumMotorPosition::HIP_L:
        pulses = -encoder_hip_left;
        break;
    case EnumMotorPosition::KNEE_R:
        pulses = -encoder_knee_right;
        break;
    case EnumMotorPosition::KNEE_L:
        pulses = encoder_knee_left;
        break;
    default:
        Serial.println("Invalid motor");
        return 0.0;
    }
    return (float)pulses / PULSES_PER_REVOLUTION * 2 * PI;
}

void DataCore::setEncoderAngles(EnumMotorPosition motor, int pulse)
{
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        encoder_hip_right = pulse;
        break;
    case EnumMotorPosition::HIP_L:
        encoder_knee_left = pulse;
        break;
    case EnumMotorPosition::KNEE_R:
        encoder_knee_right = pulse;
        break;
    case EnumMotorPosition::KNEE_L:
        encoder_knee_left = pulse;
        break;
    default:
        break;
    }
}


void DataCore::resetEncoder(){
    Serial.println("\t Reseting encoder ");
    encoder_knee_right = 0;
    encoder_knee_left = 0;
    encoder_hip_right = 0;
    encoder_hip_left = 0;
    setEncoderReset(true);
}

void DataCore::setEncoderReset(bool state){
    needResetEncoder = state;
}


//BNO
float DataCore::getBnoAngle(EnumBnoPosition bno)
{
    switch (bno)
    {
    case EnumBnoPosition::EXO_BACK:
        return Imu_back;
        break;
    case EnumBnoPosition::THIGH_L:
        return Imu_thigh_left;
        break;
    case EnumBnoPosition::THIGH_R:
        return Imu_thigh_right;
        break;
    case EnumBnoPosition::TIBIA_L:
        return Imu_tibia_left;
        break;
    case EnumBnoPosition::TIBIA_R:
        return Imu_tibia_right;
        break;
    case EnumBnoPosition::MOBO:
        return 0.0f;
        break;
    default:
        Serial.println("Invalid bno pos");
        return 0.0;
    }
}

void DataCore::setBnoAngle(EnumBnoPosition bno, float angle)
{
    switch (bno)
    {
    case EnumBnoPosition::EXO_BACK:
        Imu_back = angle;
        break;
    case EnumBnoPosition::THIGH_L:
        Imu_thigh_left = angle;
        break;
    case EnumBnoPosition::THIGH_R:
        Imu_thigh_right = angle;
        break;
    case EnumBnoPosition::TIBIA_L:
        Imu_tibia_left = angle;
        break;
    case EnumBnoPosition::TIBIA_R:
        Imu_tibia_right = angle;
        break;
    case EnumBnoPosition::MOBO:
        break;
    default:
        break;
    }
}

sh2_SensorValue_t* DataCore::getBnoStruct(EnumBnoPosition bno) {
    return bnoData[static_cast<int>(bno)];
}

void DataCore::setBnoStruct(EnumBnoPosition bno, sh2_SensorValue_t* data) {
    bnoData[static_cast<int>(bno)] = data;
}

//PWM
float DataCore::getPWM(EnumMotorPosition motor){
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        return PWMRightHip;
        break;
    case EnumMotorPosition::HIP_L:
        return PWMLeftHip;
        break;
    case EnumMotorPosition::KNEE_R:
        return PWMRightKnee;
        break;
    case EnumMotorPosition::KNEE_L:
        return PWMLeftKnee;
        break;
    default:
        return 0.0;
        break;
    }
}

void DataCore::setPWM(EnumMotorPosition motor, float pwm){
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        PWMRightHip = pwm;
        break;
    case EnumMotorPosition::HIP_L:
        PWMLeftHip = pwm;
        break;
    case EnumMotorPosition::KNEE_R:
        PWMRightKnee = pwm;
        break;
    case EnumMotorPosition::KNEE_L:
        PWMLeftKnee = pwm;
        break;
    default:
        break;
    }
}

void DataCore::printAngles(){
    Serial.print("Thigh left: ");
    Serial.print(Imu_thigh_left);
    Serial.print("  Tibia left: ");
    Serial.print(Imu_tibia_left);
    Serial.print("  Back: ");
    Serial.print(Imu_back);
}


