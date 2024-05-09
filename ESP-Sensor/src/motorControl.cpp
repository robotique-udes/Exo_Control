#include "motorControl.h"
bool motorMode = OFF; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test
double power = 175;

Motor::Motor()
{
}

Motor::~Motor()
{
}

void Motor::setPins()
{

  pinExtender.pinMode(D1_CT_A, INPUT);
  pinExtender.pinMode(D2_CT_A, INPUT);
  pinExtender.pinMode(D1_CT_B, INPUT);
  pinExtender.pinMode(D2_CT_B, INPUT);

  pinExtender.pinMode(D1_IN1_A, OUTPUT);
  pinExtender.pinMode(D1_IN2_A, OUTPUT);
  pinExtender.pinMode(D2_IN1_A, OUTPUT);
  pinExtender.pinMode(D2_IN2_A, OUTPUT);
  pinExtender.pinMode(D1_IN1_B, OUTPUT);
  pinExtender.pinMode(D1_IN2_B, OUTPUT);
  pinExtender.pinMode(D2_IN1_B, OUTPUT);
  pinExtender.pinMode(D2_IN2_B, OUTPUT);

  // TODO : put in pin extender when pwm pin extender is working
  pinMode(D1_EN_A, OUTPUT);
  pinMode(D2_EN_A, OUTPUT);
  pinMode(D1_EN_B, OUTPUT);
  pinMode(D2_EN_B, OUTPUT);

}
void Motor::readCurrent()
{
  int CTLeftKnee = D1_CT_A;
  int CTRightKnee = D2_CT_A;
  int CTLeftHip = D1_CT_B;
  int CTRightHip = D2_CT_B;
  float CTRightKneeValue = 0.0;
  float CTLeftKneeValue = 0.0;
  float CTRightHipValue = 0.0;
  float CTLeftHipValue = 0.0;

  CTRightKneeValue = analogRead(CTRightKnee); // TODO : put in pin extender when pwm pin extender is working
  CTLeftKneeValue = analogRead(CTLeftKnee);   // TODO : put in pin extender when pwm pin extender is working
  CTRightHipValue = analogRead(CTRightHip);   // TODO : put in pin extender when pwm pin extender is working
  CTLeftHipValue = analogRead(CTLeftHip);     // TODO : put in pin extender when pwm pin extender is working

  RightKneeMeasuredCurrent = ((CTRightKneeValue * 27.0) / 1023.0) * 1000.0;
  LeftKneeMeasuredCurrent = ((CTLeftKneeValue * 27.0) / 1023.0) * 1000.0;
  RightHipMeasuredCurrent = ((CTRightHipValue * 27.0) / 1023.0) * 1000.0;
  LeftHipMeasuredCurrent = ((CTLeftHipValue * 27.0) / 1023.0) * 1000.0;
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
  // If clutch are on automatic, calculate torque needed
  if (motorMode)
  {
    // Right Hip Torque Equation
    if (sonar.getSonarStateR())
      RightHipTorque = 0;
    else
    {
      if (toDegrees(RightHipAngle) < 110)
      {
        RightHipTorque = -(sin(RightHipAngle) * (LF / 2.0) * (MF * G)) * 8;
      }
      else
        RightHipTorque = 0;
    }

    // Left Hip Torque Equation
    if (sonar.getSonarStateL())
      LeftHipTorque = 0;
    else
    {
      if (toDegrees(LeftHipAngle) < 110)
        LeftHipTorque = -(sin(LeftHipAngle) * (LF / 2.0) * (MF * G)) * 8;
      else
        LeftHipTorque = 0;
    }

    // Right Knee Torque Equation
    if (sonar.getSonarStateR())
    {
      if (toDegrees(RightKneeAngle) > 0)
        RightKneeTorque = ((sin(RightHipAngle) * (LF / 2) * (MF * G)) + ((sin(RightHipAngle) * LF)) * (G * MH)) * 0.5;
      else
        RightKneeAngle = 0;
    }
    else
    {
      if (toDegrees(RightKneeAngle) < 110)
        RightKneeTorque = -(sin(RightKneeAngle - RightHipAngle) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
      else
        RightKneeTorque = 0;
    }

    // Left Knee Torque Equation
    if (sonar.getSonarStateL())
    {
      if (toDegrees(LeftKneeAngle) > 0)
        LeftKneeTorque = ((sin(LeftHipAngle) * (LF / 2) * (MF * G)) + ((sin(LeftHipAngle) * LF)) * (G * MH)) * 0.5;
      else
        LeftKneeTorque = 0;
    }
    else
    {
      if (toDegrees(LeftKneeAngle) < 110)
        LeftKneeTorque = -(sin(LeftKneeAngle - LeftHipAngle) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
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
  Serial.print("  NeededTorqueHL: ");
  Serial.print(LeftHipTorque);
  Serial.print("  NeededTorqueRH: ");
  Serial.print(RightHipTorque);
  Serial.print("  NeededTorqueLK: ");
  Serial.print(LeftKneeTorque);
  Serial.print("  NeededTorqueRK: ");
  Serial.print(RightKneeTorque);
}

void Motor::neededCurrent()
{
  // neededCurrent = SideKneeTorque / TORQUE2CURRENT * 1000;

  RightKneeNeededCurrent = (RightKneeTorque / TORQUE2CURRENT) * 1000;
  LeftKneeNeededCurrent = (LeftKneeTorque / TORQUE2CURRENT) * 1000;
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

  // Setting  PWM values
  PWMRightKnee = map(RightKneeTorque, -100, 100, -power, power);
  PWMLeftKnee = map(LeftKneeTorque, -100, 100, -power, power);
  PWMRightHip = map(RightHipTorque, -100, 100, -power, power);
  PWMLeftHip = map(LeftHipTorque, -100, 100, -power, power);
}

void Motor::printSonar()
{

  Serial.print(" SL: ");
  Serial.print(sonar.getSonarStateL());
  Serial.print(" SR: ");
  Serial.print(sonar.getSonarStateR());
  Serial.print(" MM: ");
  Serial.print(motorMode);
}

void Motor::setMotorMode(bool state)
{
  motorMode = state;
}

void Motor::setPower(double p)
{
  power = p;
}

double Motor::getPower()
{
  return power;
}

void Motor::setAngle(enumIMU imuType, float val)
{
  switch (imuType)
  {
  case enumIMU::HipR:
    RightHipAngle = val;
    break;
  case enumIMU::KneeR:
    RightKneeAngle = val;
    break;
  case enumIMU::HipL:
    LeftHipAngle = val;
    break;
  case enumIMU::KneeL:
    LeftKneeAngle = val;
    break;
  default:
    break;
  }
}

float Motor::toDegrees(float radians)
{
  return radians * 180 / PI;
}

void Motor::setSonarState(bool state) { sonar.setSonarState(state); }
void Motor::setHeight(double h) { sonar.setHeight(h); }
double Motor::sonarScanR() { return sonar.sonarScanR(); }
double Motor::sonarScanL() { return sonarScanL(); }
void Motor::sonarRead() { sonar.sonarRead(); }

void Motor::motorSetSpeed(int ID, int in1, int in2, int val)
{
  int IN1;
  int IN2;
  int EN;

  if (ID == MOTEUR_GENOU_GAUCHE)
  {
    IN1 = D1_IN1_A;
    IN2 = D1_IN2_A;
    EN = D1_EN_A;
  }
  else if (ID == MOTEUR_HANCHE_GAUCHE)
  {
    IN1 = D2_IN1_A;
    IN2 = D2_IN2_A;
    EN = D2_EN_A;
  }
  else if (ID == MOTEUR_GENOU_DROIT)
  {
    IN1 = D1_IN1_B;
    IN2 = D1_IN2_B;
    EN = D1_EN_B;
  }
  else if (ID == MOTEUR_HANCHE_DROITE)
  {
    IN1 = D2_IN1_B;
    IN2 = D2_IN2_B;
    EN = D2_EN_B;
  }
  else
  {
    Serial.println("ERROR : ID MOTEUR INCORRECT");
  }

  pinExtender.digitalWrite(IN1, in1);
  pinExtender.digitalWrite(IN2, in2);
  pinExtender.digitalWrite(EN, val); // TODO : analogWrite instead of digitalWrite when pwm pin extender is working
}

void Motor::sendCommand()
{
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, PWMLeftKnee < 0 ? 0 : 1, PWMLeftKnee < 0 ? 1 : 0, PWMLeftKnee);
  motorSetSpeed(MOTEUR_GENOU_DROIT, PWMRightKnee < 0 ? 0 : 1, PWMRightKnee < 0 ? 1 : 0, PWMRightKnee);
  motorSetSpeed(MOTEUR_HANCHE_GAUCHE, PWMLeftHip < 0 ? 0 : 1, PWMLeftHip < 0 ? 1 : 0, PWMLeftHip);
  motorSetSpeed(MOTEUR_HANCHE_DROITE, PWMRightHip < 0 ? 0 : 1, PWMRightHip < 0 ? 1 : 0, PWMRightHip);
}
