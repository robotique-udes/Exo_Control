#include "motorControl.h"
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
void Motor::readCurrent()
{
  RightKneeMeasuredCurrent = ((analogRead(PIN_MD_GEN_DRO_CT) * 27.0) / 1023.0) * 1000.0;
  LeftKneeMeasuredCurrent = ((analogRead(PIN_MD_GEN_GAU_CT) * 27.0) / 1023.0) * 1000.0;
  RightHipMeasuredCurrent = ((analogRead(PIN_MD_HAN_DRO_CT) * 27.0) / 1023.0) * 1000.0;
  LeftHipMeasuredCurrent = ((analogRead(PIN_MD_HAN_GAU_CT) * 27.0) / 1023.0) * 1000.0;
}

void Motor::LimitMinMaxFloat(float &val, float max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::LimitMinMaxInt(int &val, int max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::neededTorque()
{
  float LeftHipRAD = toRadian(LeftHipAngle);
  float LeftKneeRAD = toRadian(LeftKneeAngle);
  float RightHipRAD = toRadian(RightHipAngle);
  float RightKneeRAD = toRadian(RightKneeAngle);

  // If clutch are on automatic, calculate torque needed
  if (dataCore.isMotorEnabled())
  {
    // Right Hip Torque Equation
    if (!dataCore.getRightProxi())
      RightHipTorque = (MF * G) * sin(RightHipRAD) * LF / 2 + (MT * G) * (sin(RightHipRAD) * LF + sin(RightKneeRAD - RightHipRAD) * LT / 2); // TODO : TEST THIS NEW EQUATION
    else
    {
      if (RightHipAngle < 110)
      {
        RightHipTorque = -(sin(RightHipRAD) * (LF / 2.0) * (MF * G)) * 8;
      }
      else
        RightHipTorque = 0;
    }

    // Left Hip Torque Equation
    if (!dataCore.getLeftProxi())
      LeftHipTorque = (MF * G) * sin(LeftHipRAD) * LF / 2 + (MT * G) * (sin(LeftHipRAD) * LF + sin(LeftKneeRAD - LeftHipRAD) * LT / 2); // TODO : TEST THIS NEW EQUATION
    else
    {
      if (LeftHipAngle < 110)
        LeftHipTorque = -(sin(LeftHipRAD) * (LF / 2.0) * (MF * G)) * 8;
      else
        LeftHipTorque = 0;
    }

    // Right Knee Torque Equation
    if (!dataCore.getRightProxi())
    {
      if (RightKneeAngle > 0)
        RightKneeTorque = ((sin(RightHipRAD) * (LF / 2) * (MF * G)) + ((sin(RightHipRAD) * LF)) * (G * MH)) * 0.5;
      else
        RightKneeAngle = 0;
    }
    else
    {
      if (RightKneeAngle < 110)
        RightKneeTorque = -(sin(RightKneeRAD - RightHipRAD) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
      else
        RightKneeTorque = 0;
    }

    // Left Knee Torque Equation
    if (!dataCore.getLeftProxi())
    {
      if (LeftKneeAngle > 0)
        LeftKneeTorque = ((sin(LeftHipRAD) * (LF / 2) * (MF * G)) + ((sin(LeftHipRAD) * LF)) * (G * MH)) * 0.5;
      else
        LeftKneeTorque = 0;
    }
    else
    {
      if (LeftKneeAngle < 110)
        LeftKneeTorque = -(sin(LeftKneeRAD - LeftHipRAD) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
      else
        LeftKneeTorque = 0;
    }
  }
  else
  {
    LeftHipTorque = 0;
    RightHipTorque = 0;
    LeftKneeTorque = 0;
    RightKneeTorque = 0;
  }
}

void Motor::printTorque()
{
  Serial.print("  NeededTorqueHL: \t");
  Serial.print(LeftHipTorque);
  Serial.print("  NeededTorqueRH: \t");
  Serial.print(RightHipTorque);
  Serial.print("  NeededTorqueLK: \t");
  Serial.print(LeftKneeTorque);
  Serial.print("  NeededTorqueRK: \t");
  Serial.println(RightKneeTorque);
}

void Motor::neededCurrent()
{
  RightKneeNeededCurrent = (RightKneeTorque / TORQUE2CURRENT) * 1000;
  LeftKneeNeededCurrent = (LeftKneeTorque / TORQUE2CURRENT) * 1000;
  RightHipNeededCurrent = (RightHipTorque / TORQUE2CURRENT) * 1000;
  LeftHipNeededCurrent = (LeftHipTorque / TORQUE2CURRENT) * 1000;
}

void Motor::PIDCurrent()
{
  // TODO : In the future, use PID control instead of direct PWM control using map in PIDCurrentPrealable
  //        For now, the PID control is not working properly, so we use the map function to set the PWM values

  /*
    // Set error values on measured current compared to needed current determined by torque
    ErrorCurrentRightKnee = RightKneeNeededCurrent - RightKneeMeasuredCurrent;
    ErrorCurrentLeftKnee = LeftKneeNeededCurrent - LeftKneeMeasuredCurrent;

    // Update integral values depending on calculated errors
    IntegralRightKnee += ErrorCurrentRightKnee;
    IntegralLeftKnee += ErrorCurrentLeftKnee;

    // Update derivative values depending on the rate of change of the calculated errors
    DerivativeRightKnee = ErrorCurrentRightKnee - PreviousErrorRightKnee;
    DerivativeLeftKnee = ErrorCurrentLeftKnee - PreviousErrorLeftKnee;

    // Capping the error values
    LimitMinMaxFloat(ErrorCurrentRightKnee, 5);
    LimitMinMaxFloat(ErrorCurrentLeftKnee, 5);

    // LimitMinMaxFloat(derivative, 50);
    // Setting both PWM values
    PWMRightKnee += KP * ErrorCurrentRightKnee + KI * IntegralRightKnee + KD * DerivativeRightKnee;
    PWMLeftKnee += KP * ErrorCurrentLeftKnee + KI * IntegralLeftKnee + KD * DerivativeLeftKnee;

    // Setting the previous errors for both motors
    PreviousErrorRightKnee = ErrorCurrentRightKnee;
    PreviousErrorLeftKnee = ErrorCurrentLeftKnee;

    // Capping the PWM values for both motors
    LimitMinMaxInt(PWMRightKnee, 255);
    LimitMinMaxInt(PWMLeftKnee, 255);

    //if (angle < 0.2 || angle > (2 * PI - 0.2))
      //PWM = 0;

    */
}

void Motor::PIDCurrentPrealable()
{
  int power = dataCore.getMotorPower();

  // Setting  PWM values
  PWMRightKnee = -float(map(RightKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMLeftKnee = float(map(LeftKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMRightHip = float(map(RightHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMLeftHip = -float(map(LeftHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
}

void Motor::printPMW()
{
  Serial.print("\t Motor Power: ");
  Serial.print(dataCore.getMotorPower());
  Serial.print("\t  PWMRightKnee: ");
  Serial.print(PWMRightKnee);
  Serial.print("\t  PWMLeftKnee: ");
  Serial.print(PWMLeftKnee);
  Serial.print("\t  PWMRightHip: ");
  Serial.print(PWMRightHip);
  Serial.print("\t  PWMLeftHip: ");
  Serial.println(PWMLeftHip);
}


void Motor::setAngle(enumIMU imuType, float val)
{
  switch (imuType)
  {
  case enumIMU::HIP_R:
    RightHipAngle = val;
    break;
  case enumIMU::KNEE_R:
    RightKneeAngle = val;
    break;
  case enumIMU::HIP_L:
    LeftHipAngle = val;
    break;
  case enumIMU::KNEE_L:
    LeftKneeAngle = val;
    break;
  default:
    break;
  }
}

float Motor::getAngle(enumIMU imuType)
{
  switch (imuType)
  {
  case enumIMU::HIP_R:
    return RightHipAngle;
    break;
  case enumIMU::KNEE_R:
    return RightKneeAngle;
    break;
  case enumIMU::HIP_L:
    return LeftHipAngle;
    break;
  case enumIMU::KNEE_L:
    return LeftKneeAngle;
    break;
  default:
    return 0;
    break;
  }
}

float Motor::toDegrees(float radians)
{
  return radians * 180 / PI;
}

float Motor::toRadian(float degree)
{
  return degree * PI / 180;
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

void Motor::sendCommand()
{
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, PWMLeftKnee);
  motorSetSpeed(MOTEUR_GENOU_DROIT, PWMRightKnee);
  motorSetSpeed(MOTEUR_HANCHE_GAUCHE, PWMLeftHip);
  motorSetSpeed(MOTEUR_HANCHE_DROITE, PWMRightHip);
}

void Motor::testFuncSetPwnHigh()
{
  pwmPinExtender.setChannelPWM(4, 2048);
  pwmPinExtender.setChannelPWM(5, 2048);
  pwmPinExtender.setChannelPWM(6, 2048);
  pwmPinExtender.setChannelPWM(7, 2048);
}

// A renommer correctement
void Motor::setSonarState(bool state) { dataCore.setProximEnabled(state); }
void Motor::setHeight(double h) { dataCore.setHeight(h); }
// double Motor::sonarScanR(){ return RightProxim->GetMinDistance(); }
// double Motor::sonarScanL(){ return LeftProxim->GetMinDistance(); }
