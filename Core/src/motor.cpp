#include "motor.h"
bool motorMode = ON; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test

Motor::Motor()
{

}

Motor::~Motor()
{
}

void Motor::setPins()
{
  pinMode(PIN_MD_HAN_GAU_CT, INPUT);
  pinMode(PIN_MD_HAN_DRO_CT, INPUT);
  pinMode(PIN_MD_GEN_GAU_CT, INPUT);
  pinMode(PIN_MD_GEN_DRO_CT, INPUT);

  pinExtender.pinMode(PIN_MD_GEN_GAU_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_GEN_DRO_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_HAN_GAU_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_HAN_DRO_EN, OUTPUT);
}

void Motor::write()
{
  if(dataCore.isMotorEnabled()){
    motorSetSpeed(MOTEUR_GENOU_GAUCHE, dataCore.getPWM(EnumMotorPosition::KNEE_L));
    motorSetSpeed(MOTEUR_GENOU_DROIT, dataCore.getPWM(EnumMotorPosition::KNEE_R));
    motorSetSpeed(MOTEUR_HANCHE_GAUCHE, dataCore.getPWM(EnumMotorPosition::HIP_L));
    motorSetSpeed(MOTEUR_HANCHE_DROITE, dataCore.getPWM(EnumMotorPosition::HIP_R));
  }
}

void Motor::motorSetSpeed(int ID, int val)
{
  // ID 0 to 3
  // val -4096 to 4096

  if (val > 4000)
    val = 4000; // Protection contre les valeurs trop élevées (drive limitée à 98%)
  else if (val < -4000)
    val = -4000;

  // deadzone à 200
  if (val < 200 && val > -200)
    val = 0;

  int PIN_IN1;
  int PIN_IN2;
  int PIN_EN;

  if (ID == MOTEUR_GENOU_GAUCHE)
  {
    PIN_IN1 = PIN_MD_GEN_GAU_INA;
    PIN_IN2 = PIN_MD_GEN_GAU_INB;
    PIN_EN = PIN_MD_GEN_GAU_EN;
  }
  else if (ID == MOTEUR_HANCHE_GAUCHE)
  {
    PIN_IN1 = PIN_MD_HAN_GAU_INA;
    PIN_IN2 = PIN_MD_HAN_GAU_INB;
    PIN_EN = PIN_MD_HAN_GAU_EN;
  }
  else if (ID == MOTEUR_GENOU_DROIT)
  {
    PIN_IN1 = PIN_MD_GEN_DRO_INA;
    PIN_IN2 = PIN_MD_GEN_DRO_INB;
    PIN_EN = PIN_MD_GEN_DRO_EN;
  }
  else if (ID == MOTEUR_HANCHE_DROITE)
  {
    PIN_IN1 = PIN_MD_HAN_DRO_INA;
    PIN_IN2 = PIN_MD_HAN_DRO_INB;
    PIN_EN = PIN_MD_HAN_DRO_EN;
  }
  else
  {
    Serial.println("ERROR : ID MOTEUR INCORRECT");
  }

  if (val > 0)
  {
    pwmPinExtender.setChannelPWM(PIN_IN1, val);
    pwmPinExtender.setChannelPWM(PIN_IN2, OFF);
    pinExtender.digitalWrite(PIN_EN, ON);
  }
  else if (val < 0)
  {
    pwmPinExtender.setChannelPWM(PIN_IN1, OFF);
    pwmPinExtender.setChannelPWM(PIN_IN2, -val);
    pinExtender.digitalWrite(PIN_EN, ON);
  }
  else
  {
    pwmPinExtender.setChannelPWM(PIN_IN1, OFF);
    pwmPinExtender.setChannelPWM(PIN_IN2, OFF);
    pinExtender.digitalWrite(PIN_EN, OFF);
  }
}

void Motor::testFuncSetPwnHigh()
{
  pwmPinExtender.setChannelPWM(4, 2048);
  pwmPinExtender.setChannelPWM(5, 2048);
  pwmPinExtender.setChannelPWM(6, 2048);
  pwmPinExtender.setChannelPWM(7, 2048);
}

void Motor::printPMW()
{
    Serial.print("\t Motor Power: ");
    Serial.print(dataCore.getMotorPower());
    Serial.print("\t  PWMRightKnee: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::KNEE_R));
    Serial.print("\t  PWMLeftKnee: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::KNEE_L));
    Serial.print("\t  PWMRightHip: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::HIP_R));
    Serial.print("\t  PWMLeftHip: ");
    Serial.println(dataCore.getPWM(EnumMotorPosition::HIP_L));
}
