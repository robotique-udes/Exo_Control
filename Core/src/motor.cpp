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