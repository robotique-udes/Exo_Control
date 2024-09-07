#include "dataCore.h"

DataCore *DataCore::instance;

DataCore::DataCore(){
    rightProxiState = false;
    leftProxiState = false;
    motorEnabled = false;
    clutchEnabled = false;
    proximEnabled = true;
    brightness = HIGH;
    needResetProxim = false;

    sonarState = SQUAT_MODE;
    angleSource = FROM_IMU;

    height = 180;
    motorPower = 2048;
}

void DataCore::initialise(){
    clutchEnabled = OFF;
    motorEnabled = false;
    motorPower = 2048;
    needResetProxim = true;
    sonarState = FROM_IMU;
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

bool DataCore::isProximEnabled(){
    return proximEnabled;
}

bool DataCore::isEncoderResetNeeded(){
    return needResetEncoder;
}

void DataCore::setMotorEnabled(bool setMotorEnabled){
    motorEnabled = !(motorEnabled);
    Serial.print("\t Motor enable set to: ");
    Serial.println(motorEnabled);
}

void DataCore::setClutchEnabled(bool setClutchEnabled){
    clutchEnabled = !(clutchEnabled);
    relais.setAllRelay(clutchEnabled);
    Serial.print("\t Clutch enable set to: ");
    Serial.println(clutchEnabled);
}

void DataCore::setProximEnabled(bool setProximEnabled){
    proximEnabled = !(proximEnabled);
    Serial.print("\t Proxim enable set to: ");
    Serial.println(proximEnabled);
}

bool DataCore::getAngleSource(){
    return angleSource;
}

void DataCore::setAngleSource(bool setAngleSource){
    angleSource = (setAngleSource);
    Serial.print("\t Angle source set to: ");
    Serial.println(angleSource);
}


//PROXIM
bool DataCore::getBrightness(){
    return brightness;
}

void DataCore::setBrightness(){
    brightness = !(brightness);
}

bool DataCore::getResetProxim(){
    return needResetProxim;
}

bool DataCore::getRightProxi(){
    return rightProxiState;
}

void DataCore::setRightProxi(bool state){
    rightProxiState = state;
}

bool DataCore::getLeftProxi(){
    return leftProxiState;
}

void DataCore::setLeftProxi(bool state){
    leftProxiState = state;
}

void DataCore::setResetProxim(bool reset){
    needResetProxim = reset;
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
float DataCore::getBnoAngles(EnumBnoAngle bno)
{
    switch (bno)
    {
    case EnumBnoAngle::HIP_R:
        return Imu_hip_right;
        break;
    case EnumBnoAngle::HIP_L:
        return Imu_hip_left;
        break;
    case EnumBnoAngle::KNEE_R:
        return Imu_knee_right;
        break;
    case EnumBnoAngle::KNEE_L:
        return Imu_knee_left;
        break;
    case EnumBnoAngle::EXO_BACK:
        return Imu_back;
        break;
    case EnumBnoAngle::THIGH_L:
        return Imu_thigh_left;
        break;
    case EnumBnoAngle::THIGH_R:
        return Imu_thigh_right;
        break;
    case EnumBnoAngle::TIBIA_L:
        return Imu_tibia_left;
        break;
    case EnumBnoAngle::TIBIA_R:
        return Imu_tibia_right;
        break;
    default:
        Serial.println("Invalid bno pos");
        return 0.0;
    }
}

void DataCore::setBnoAngles(EnumBnoAngle bno, float angle)
{
    switch (bno)
    {
    case EnumBnoAngle::HIP_R:
        Imu_hip_right = angle;
        break;
    case EnumBnoAngle::HIP_L:
        Imu_hip_left = angle;
        break;
    case EnumBnoAngle::KNEE_R:
        Imu_knee_right = angle;
        break;
    case EnumBnoAngle::KNEE_L:
        Imu_knee_left = angle;
        break;
    case EnumBnoAngle::EXO_BACK:
        Imu_back = angle;
        break;
    case EnumBnoAngle::THIGH_L:
        Imu_thigh_left = angle;
        break;
    case EnumBnoAngle::THIGH_R:
        Imu_thigh_right = angle;
        break;
    case EnumBnoAngle::TIBIA_L:
        Imu_tibia_left = angle;
        break;
    case EnumBnoAngle::TIBIA_R:
        Imu_tibia_right = angle;
        break;
    default:
        break;
    }
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


