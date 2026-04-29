#include "dataCore.h"

DataCore *DataCore::instance;

DataCore::DataCore(){
    
    groundDetectEnable = true;
    initialise();
    bnoData = {nullptr, nullptr, nullptr, nullptr, nullptr};
}

void DataCore::initialise(){
    rightGrounded = false;
    leftGrounded = false;
    motorEnabled = false;
    motorPower = 2048;
    angleSource = FROM_IMU;
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

bool DataCore::isGroundDetectEnable(){
    return groundDetectEnable;
}

//TODO when hmi is refactored, change toggle to using input param
void DataCore::setMotorEnabled(bool setMotorEnabled){
    motorEnabled = setMotorEnabled;
    Serial.print("Motor state: ");
    Serial.println(isMotorEnabled());
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

BnoData_t* DataCore::getBnoStruct(EnumBnoPosition bno) {
    return bnoData[static_cast<int>(bno)];
}

void DataCore::setBnoStruct(EnumBnoPosition bno, BnoData_t* data) {
    bnoData[static_cast<int>(bno)] = data;
}

//PWM
float DataCore::getTorque(EnumMotorPosition motor){
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        return torqueRightHip;
        break;
    case EnumMotorPosition::HIP_L:
        return torqueLeftHip;
        break;
    case EnumMotorPosition::KNEE_R:
        return torqueRightKnee;
        break;
    case EnumMotorPosition::KNEE_L:
        return torqueLeftKnee;
        break;
    default:
        return 0.0;
        break;
    }
}

void DataCore::setTorque(EnumMotorPosition motor, float torque){
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        torqueRightHip = torque;
        break;
    case EnumMotorPosition::HIP_L:
        torqueLeftHip = torque;
        break;
    case EnumMotorPosition::KNEE_R:
        torqueRightKnee = torque;
        break;
    case EnumMotorPosition::KNEE_L:
        torqueLeftKnee = torque;
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


