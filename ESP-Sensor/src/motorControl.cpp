#include "motorControl.h"
double height = 40;
bool motorMode = ON;
// ExponentialFilter<float> FilteredCurrent(7, 0);

Motor::Motor()
{
}

Motor::~Motor()
{
}

// Servo MonServo;

/*Fonctions de torque a integrer

TgSol = (((cos(angleGenoux)*LF) - 0.5*LH*cos(angleHanche))*Fgh) + (0.5*LF*cos(angleGenoux)*Fgf);
TgVide = (cos(angleGenoux)*Fgt*LG)/2;


ThSol = (0.5*LH*cos(angleHanche)*Fgh);
ThVide = (cos(angleHanche)*LH*Fgf)/2 + (cos(angleHanche)*LF + (cos(angleGenoux)*LG)/2)*Fgt;
*/


void Motor::setPins()
{
  // PINS MOTEURS
  //pinMode(D1_IN1_A, OUTPUT);
  // pinMode(D1_IN2_A, OUTPUT);
  // pinMode(D1_EN_A, OUTPUT);
  // pinMode(D1_CT_A, INPUT);
  // pinMode(D2_IN1_A, OUTPUT);
  // pinMode(D2_IN2_A, OUTPUT);
  // pinMode(D2_EN_A, OUTPUT);
  // pinMode(D2_CT_A, INPUT);

  // PINS RELAIS
  pinMode(RELAIS_PIN_GENOU_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_GENOU_DROIT, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_DROITE, OUTPUT);

  // PINS SONAR
  pinMode(TRIG_PIN_GAUCHE, OUTPUT);
  pinMode(ECHO_PIN_GAUCHE, INPUT);
  pinMode(TRIG_PIN_DROIT, OUTPUT);
  pinMode(ECHO_PIN_DROIT, INPUT);
}


void Motor::readCurrent()
{
  int CTLeftKnee = D1_CT_A;;
  int CTRightKnee = D2_CT_A;
  float CTRightKneeValue = 0.0;
  float CTLeftKneeValue = 0.0;

  CTRightKneeValue = analogRead(CTRightKnee);
  CTLeftKneeValue = analogRead(CTLeftKnee);

  RightKneeMeasuredCurrent = ((CTRightKneeValue * 27.0) / 1023.0) * 1000.0;
  LeftKneeMeasuredCurrent = ((CTLeftKneeValue * 27.0) / 1023.0) * 1000.0; 
}


void Motor::CapperFloat(float &val, float max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::CapperInt(int &val, int max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::neededTorque()
{
  //If clutch are on automatic, calculate torque needed
  if(!motorMode){
    // Right Hip Torque Equation
    if (RightSonarState)
      RightHipTorque = (0.5*LH*cos(RightHipAngle)*G*MH);
    else
      RightHipTorque = -(cos(RightHipAngle)*LF*G*MF)/2 - (cos(RightHipAngle)*LF + (cos(RightKneeAngle)*LT)/2)*G*MT;  

    // Left Hip Torque Equation
    if (LeftSonarState)
      LeftHipTorque = (0.5*LH*cos(LeftHipAngle)*G*MH);
    else
      LeftHipTorque = -(cos(LeftHipAngle)*LF*G*MF)/2 - (cos(LeftHipAngle)*LF + (cos(LeftKneeAngle)*LT)/2)*G*MT;    

    // Right Knee Torque Equation
    if (RightSonarState)
      RightKneeTorque = ((sin(RightKneeAngle)*LF)/2.0)*(MF*G) + ((sin(RightKneeAngle)*LF))*(MH*G);
    else
      RightKneeTorque = -((sin(RightKneeAngle)*LT)/2.0)*(MT*G);  
      
    // Left Knee Torque Equation
    if (LeftSonarState)
      LeftKneeTorque = ((sin(LeftKneeAngle)*LF)/2.0)*(MF*G) + ((sin(LeftKneeAngle)*LF))*(MH*G);
    else
      LeftKneeTorque = -((sin(LeftKneeAngle)*LT)/2.0)*(MT*G);
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
  Serial.print("  RightHipAngle: ");
  Serial.print(toDegrees(RightHipAngle));
  Serial.print("  LeftHipAngle: ");
  Serial.print(toDegrees(LeftHipAngle));

  Serial.print("  RightKneeAngle: ");
  Serial.print(toDegrees(RightKneeAngle));
  Serial.print("  LeftKneeAngle: ");
  Serial.print(toDegrees(LeftKneeAngle));

  Serial.print("  NeededTorqueL: ");
  Serial.print(LeftKneeTorque);
  Serial.print("  NeededTorqueR: ");
  Serial.print(RightKneeTorque);
}

void Motor::neededCurrent()
{
  //neededCurrent = SideKneeTorque / TORQUE2CURRENT * 1000;

RightKneeNeededCurrent = (RightKneeTorque/TORQUE2CURRENT)*1000;
LeftKneeNeededCurrent = (LeftKneeTorque/TORQUE2CURRENT)*1000;

}

void Motor::PIDCurrent()
{
//Set error values on measured current compared to needed current determined by torque
  ErrorCurrentRightKnee = RightKneeNeededCurrent - RightKneeMeasuredCurrent;
  ErrorCurrentLeftKnee = LeftKneeNeededCurrent - LeftKneeMeasuredCurrent;

//Update integral values depending on calculated errors
  IntegralRightKnee += ErrorCurrentRightKnee;
  IntegralLeftKnee += ErrorCurrentLeftKnee;

//Update derivative values depending on the rate of change of the calculated errors 
  DerivativeRightKnee = ErrorCurrentRightKnee - PreviousErrorRightKnee;
  DerivativeLeftKnee = ErrorCurrentLeftKnee - PreviousErrorLeftKnee;

//Capping the error values
  CapperFloat(ErrorCurrentRightKnee, 5);
  CapperFloat(ErrorCurrentLeftKnee, 5);

  // CapperFloat(derivative, 50);
//Setting both PWM values 
  PWMRightKnee += KP * ErrorCurrentRightKnee + KI * IntegralRightKnee + KD * DerivativeRightKnee;
  PWMLeftKnee += KP * ErrorCurrentLeftKnee + KI * IntegralLeftKnee + KD * DerivativeLeftKnee;

//Setting the previous errors for both motors
  PreviousErrorRightKnee = ErrorCurrentRightKnee;
  PreviousErrorLeftKnee = ErrorCurrentLeftKnee;

//Capping the PWM values for both motors
  CapperInt(PWMRightKnee, 255);
  CapperInt(PWMLeftKnee, 255);

  /*if (angle < 0.2 || angle > (2 * PI - 0.2))
    PWM = 0;*/

}

void Motor::PIDCurrentPrealable()
{

//Setting  PWM values 
  PWMRightKnee = map(RightKneeTorque, -100, 100, -255, 255);
  PWMLeftKnee = map(LeftKneeTorque, -100, 100, -255, 255);
  PWMRightHip = map(RightHipTorque, -100, 100, -255, 255);
  PWMLeftHip = map(LeftHipTorque, -100, 100, -255, 255);

}

/*void Motor::printData(long Count_pulses)
{
  // Serial.print(" Angle: ");
  // Serial.print(angle);
  Serial.print(" Courant GRAVITE: ");
  Serial.print(CourantSouhaite);
  // Serial.print(" derivate: ");
  // Serial.print(derivative);
  Serial.print(" Courant actuel: ");
  //Serial.print(ReadCurrent(MOTEUR_GENOU_DROIT));
  // Serial.print(" Count_pulses: ");
  // Serial.print(Count_pulses);
  Serial.print(" PWM: ");
  Serial.println(PWM);
}*/
double Motor::sonarScanL()
{
  //Signal acquisition from left sonar
  digitalWrite(TRIG_PIN_GAUCHE, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN_GAUCHE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_GAUCHE, LOW);
  double LeftDuration = pulseIn(ECHO_PIN_GAUCHE, HIGH);
  return (LeftDuration / 2) / 29.1;

}

double Motor::sonarScanR()
{
  //Signal acquisition from right sonar
  digitalWrite(TRIG_PIN_DROIT, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN_DROIT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_DROIT, LOW);
  double RightDuration = pulseIn(ECHO_PIN_DROIT, HIGH);

  return (RightDuration / 2) / 29.1;
}
void Motor::sonarRead()
{

  float errorRight = 0;
  float errorLeft = 0;


//Determining the current SonarState for each sensor

//Here, right sensor is examined 
  if (!RightSonarState)
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanR() < height)
        errorRight += 1;
    errorRight /= iteration;
    if (errorRight <= UNCERTAINTY)
      RightSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanR() > height)
        errorRight += 1;
    errorRight /= iteration;
    if (errorRight <= UNCERTAINTY)
      RightSonarState = false;
  }

//Here, left sensor is examined
   if (!LeftSonarState)
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanL() < height)
        errorLeft += 1;
    errorLeft /= iteration;
    if (errorLeft <= UNCERTAINTY)
      LeftSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanL() > height)
        errorLeft += 1;
    errorLeft /= iteration;
    if (errorLeft <= UNCERTAINTY)
      LeftSonarState = false;
  }
  
  LeftSonarState = !LeftSonarState;
  RightSonarState = !RightSonarState;


}
void Motor::printSonar()
{
  Serial.print(" SL: ");
  Serial.print(LeftSonarState);
  Serial.print(" SR: ");
  Serial.print(RightSonarState);
}

void Motor::setRelais(int ID, bool state)
{
  if (ID == RELAIS_GENOU_GAUCHE)
  {
    if (state != ON)
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, LOW);
  }
  else if (ID == RELAIS_GENOU_DROIT)
  {
    if (state != ON)
      digitalWrite(RELAIS_PIN_GENOU_DROIT, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_DROIT, LOW);
  }
  else if (ID == RELAIS_HANCHE_GAUCHE)
  {
    if (state != ON)
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, LOW);
  }
  else if (ID == RELAIS_HANCHE_DROITE)
  {
    if (state != ON)
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, LOW);
  }
}
void Motor::setAllRelais(bool state)
{
  Serial.print("switching all relay to: ");
  Serial.println(state);
   setRelais(RELAIS_GENOU_GAUCHE, state);
   setRelais(RELAIS_GENOU_DROIT,state);
   setRelais(RELAIS_HANCHE_DROITE,state);
   setRelais(RELAIS_HANCHE_GAUCHE,state);
   motorMode = state; 
}

void Motor::testRelais()
{
  // Test Relais
  Serial.print("Relais 1: ");
  setRelais(RELAIS_GENOU_GAUCHE, ON);
  delay(1000);
  setRelais(RELAIS_GENOU_GAUCHE, OFF);
  Serial.print("Relais 2: ");
  setRelais(RELAIS_GENOU_DROIT, ON);
  delay(1000);
  setRelais(RELAIS_GENOU_DROIT, OFF);
  Serial.print("Relais 3: ");
  setRelais(RELAIS_HANCHE_GAUCHE, ON);
  delay(1000);
  setRelais(RELAIS_HANCHE_GAUCHE, OFF);
  Serial.print("Relais 4: ");
  setRelais(RELAIS_HANCHE_DROITE, ON);
  delay(1000);
  setRelais(RELAIS_HANCHE_DROITE, OFF);
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

void Motor::setHeight(double h)
{
  height = h;
  Serial.print("Height set to: ");
  Serial.println(h);
}

double Motor::getHeight()
{
  return height;
}
