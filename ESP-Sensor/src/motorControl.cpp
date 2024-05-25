#include "motorControl.h"
bool motorMode = ON; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test

Motor::Motor()
{
  this->mux = Multiplex();
  this->LeftProxim = new ProxiSensor(&this->mux, LEFT_MOUSTACHE_MUX_CHANNEL);
  this->RightProxim = new ProxiSensor(&this->mux, RIGHT_MOUSTACHE_MUX_CHANNEL);

  // Fill data sets
  for (int i = 0; i < 5; i++)
  {
    //
    for (int j = 0; j < PREDICTION_LENGTH; j++)
    {
      angles[i][j] = 0;
      times[i][j] = 0;
    }
    anglesIndex[i] = 0;
  }
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
  // check if proxim to reset trigger dist
  if (settings.getResetProxim())
  {
    SetTriggerDistance();
    settings.setResetProxim(false);
  }
  // If clutch are on automatic, calculate torque needed
  if (settings.isMotorEnabled())
  {
    // Right Hip Torque Equation
    if (!RightProxim->IsOnTheGround())
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
    if (!LeftProxim->IsOnTheGround())
      LeftHipTorque = (MF * G) * sin(LeftHipRAD) * LF / 2 + (MT * G) * (sin(LeftHipRAD) * LF + sin(LeftKneeRAD - LeftHipRAD) * LT / 2); // TODO : TEST THIS NEW EQUATION
    else
    {
      if (LeftHipAngle < 110)
        LeftHipTorque = -(sin(LeftHipRAD) * (LF / 2.0) * (MF * G)) * 8;
      else
        LeftHipTorque = 0;
    }

    // Right Knee Torque Equation
    if (!RightProxim->IsOnTheGround())
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
    if (!LeftProxim->IsOnTheGround())
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
  // TODO : In the future, use PID control instead of direct PWM control using map in torqueToPWM
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

void Motor::printComputedSpeed(enumIMU position) {

  string posi = "DF";
  
  // Switch case for positon 
  switch (position)
  {
    case enumIMU::HIP_R:
      posi = "RH";
      break;
    case enumIMU::KNEE_R:
      posi = "RK";
      break;
    case enumIMU::HIP_L:  
      posi = "LH";
      break;
    case enumIMU::KNEE_L:
      posi = "LK";
      break;
    default:
      break;
  }


  // Print data
  Serial.print(posi.c_str());
  Serial.print("A:\t");
  Serial.print(getAngle(position));
  Serial.print("\t");
  Serial.print(posi.c_str());
  Serial.print("S:\t");
  Serial.print(getSpeed(position));
  Serial.print("\t");
  Serial.print(posi.c_str());
  Serial.print("M:\t");
  Serial.println(computePWMMultiplier(position));
}

void Motor::torqueToPWM()
{
  int power = settings.getMotorPower();

  float multiRightKnee = computePWMMultiplier(enumIMU::KNEE_R);
  float multiLeftKnee = computePWMMultiplier(enumIMU::KNEE_L);
  float multiRightHip = computePWMMultiplier(enumIMU::HIP_R);
  float multiLeftHip = computePWMMultiplier(enumIMU::HIP_L);



  // Setting  PWM values
  PWMRightKnee = -float(map(RightKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMLeftKnee = float(map(LeftKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMRightHip = float(map(RightHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));
  PWMLeftHip = -float(map(LeftHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power));

  // Apply multiplier
  PWMRightKnee *= multiRightKnee;
  PWMLeftKnee *= multiLeftKnee;
  PWMRightHip *= multiRightHip;
  PWMLeftHip *= multiLeftHip;
}

float Motor::getTorque(enumIMU position)
{
  switch (position)
  {
  case enumIMU::HIP_R:
    return RightHipTorque;
    break;
  case enumIMU::KNEE_R:
    return RightKneeTorque;
    break;
  case enumIMU::HIP_L:
    return LeftHipTorque;
    break;
  case enumIMU::KNEE_L:
    return LeftKneeTorque;
    break;
  default:
    return 0;
    break;
  }
}

void Motor::printPWM()
{
  Serial.print("\t Motor Power: ");
  Serial.print(settings.getMotorPower());
  Serial.print("\t  PWMRightKnee: ");
  Serial.print(PWMRightKnee);
  Serial.print("\t  PWMLeftKnee: ");
  Serial.print(PWMLeftKnee);
  Serial.print("\t  PWMRightHip: ");
  Serial.print(PWMRightHip);
  Serial.print("\t  PWMLeftHip: ");
  Serial.print(PWMLeftHip);

  Serial.print("\t RKM:\t");
  Serial.print(computePWMMultiplier(enumIMU::KNEE_R));
  Serial.print("\t RHM:\t");
  Serial.println(computePWMMultiplier(enumIMU::HIP_R));

}

void Motor::printProxim()
{
  if (settings.getResetProxim())
  {
    SetTriggerDistance();
    settings.setResetProxim(false);
  }
  Serial.print("\t RIGHT PROXIM: ");
  Serial.print(RightProxim->IsOnTheGround());
  Serial.print("\t LEFT PROXIM: ");
  Serial.println(LeftProxim->IsOnTheGround());

  // Serial.print("\t ME: ");
  // Serial.println(settings.isMotorEnabled());
}

void Motor::SetTriggerDistance()
{
  LeftProxim->SetTriggerDistance(settings.getBrightness());
  RightProxim->SetTriggerDistance(settings.getBrightness());
}

void Motor::logAngle(enumIMU position, float val)
{
  int imu = static_cast<int>(position);

  // Shift all values by one
  int currentReadingPos = this->anglesIndex[imu] % PREDICTION_LENGTH;
  this->angles[imu][currentReadingPos] = val;
  this->times[imu][currentReadingPos] = millis();
  this->anglesIndex[imu] = this->anglesIndex[imu] + 1;
}

// Returns the speed of the motor in degrees per second, positive speed means user is "crouching"
float Motor::getSpeed(enumIMU position)
{
  int imu = static_cast<int>(position);
  if (this->anglesIndex[imu] < PREDICTION_LENGTH)
  {
    return 0; // Not enough data to compute
  }

  int sumLatest = 0;   // Most recent half of the data
  int sumPrevious = 0; // Oldest half of data

  int timeLatest = 0;
  int timePrevious = 0;

  for (int i = 0; i < PREDICTION_LENGTH; i++)
  {
    int index = (this->anglesIndex[imu] - i) % PREDICTION_LENGTH; // Adjusted index
    if (i < PREDICTION_LENGTH / 2)
    {
      sumLatest += this->angles[imu][index];
      timeLatest = this->times[imu][index];
    }
    else
    {
      sumPrevious += this->angles[imu][index];
      timePrevious = this->times[imu][index]; // Storing time of oldest value
    }
  }

  // Simply get most recent, go back half the length and that space is average most recent
  float latestAngle = sumLatest / (PREDICTION_LENGTH / 2);
  float previousAngle = sumPrevious / (PREDICTION_LENGTH / 2);

  return (latestAngle - previousAngle) / ((timeLatest - timePrevious) / 1000.0);
}

// Uses the last PREDICTION_LENGTH values to compute the power of the motor
float Motor::computePWMMultiplier(enumIMU position)
{
  int imu = static_cast<int>(position);
  if (this->anglesIndex[imu] < PREDICTION_LENGTH)
  {
    return 1; // Not enough data to compute
  }

  float currentSpeed = -this->getSpeed(position);

  // Check if the speed is in the range that indicates no significant change
  if (currentSpeed < 2 && currentSpeed > -20) {
    return 1;
  }

  // Stand logic (inverted because speed symbol is flipped)
  if (currentSpeed > 0) {
    if (currentSpeed > 30) {
      // Max out the increase of power
      return 4.5;
    } else {
      // Smoothly increase power
      return 1 + (currentSpeed * 0.04);
    }
  } else { // Crouch logic (inverted because speed symbol is flipped)
    if (currentSpeed > -20) {
      return 1;
    } else if (currentSpeed >= -50) {
      // Smoothly decrease power
      return 1 + 0.025 * (currentSpeed + 20);
    } else {
      // Max out the decrease of power
      return 0.0;
    }
  }
}

void Motor::setAngle(enumIMU imuType, float val)
{
  this->logAngle(imuType, val);

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
void Motor::setSonarState(bool state) { settings.setProximEnabled(state); }
void Motor::setHeight(double h) { settings.setHeight(h); }
// double Motor::sonarScanR(){ return RightProxim->GetMinDistance(); }
// double Motor::sonarScanL(){ return LeftProxim->GetMinDistance(); }
