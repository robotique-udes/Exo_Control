#include "motorControl.h"

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
  pinMode(D1_IN2_A, OUTPUT);
  pinMode(D1_EN_A, OUTPUT);
  pinMode(D1_CT_A, INPUT);
  pinMode(D2_IN1_A, OUTPUT);
  pinMode(D2_IN2_A, OUTPUT);
  pinMode(D2_EN_A, OUTPUT);
  pinMode(D2_CT_A, INPUT);

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

void Motor::motorSetSpeed(int ID, int val)
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
  else if (ID == MOTEUR_GENOU_DROIT)
  {
    IN1 = D2_IN1_A;
    IN2 = D2_IN2_A;
    EN = D2_EN_A;
  }
  // else if(ID == MOTEUR_HANCHE_GAUCHE)
  // {
  //   IN1 = D1_IN1_B;
  //   IN2 = D1_IN2_B;
  //   EN = D1_EN_B;
  // }
  // else if(ID == MOTEUR_HANCHE_DROITE)
  // {
  //   IN1 = D2_IN1_B;
  //   IN2 = D2_IN2_B;
  //   EN = D2_EN_B;
  // }

  if (val >= 0)
  {
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
    Rotation = ClockWise;
  }
  else if (val < 0)
  {
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, HIGH);
    Rotation = CounterClockWise;
    val = -val;
  }

  analogWrite(EN, val);
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

void Motor::neededTorque()
{
  //Right knee torque values calculated (varies depending on SonarSate)(The sign of the value has to be verified here (so that the motor turns in the right direction))

  if (RightSonarState == 1)
  {
    RightKneeTorque = ((cos(90-RightHipAngle)*LF)/2.0)*(MF*G);
  }

    if (RightSonarState == 0)
  {
    RightKneeTorque = -((sin(RightKneeAngle)*LT)/2.0)*(MT*G);  
  }

//Left knee torque values calculated (varies depending on SonarSate)()

if (LeftSonarState == 1)
  {
    LeftKneeTorque = ((cos(90-LeftHipAngle)*LF)/2.0)*(MF*G); 
  }

    if (LeftSonarState == 0)
  {
    LeftKneeTorque = -((sin(LeftKneeAngle)*LT)/2.0)*(MT*G); 
  }
}

void Motor::neededCurrent()
{
  //neededCurrent = SideKneeTorque / TORQUE2CURRENT * 1000;

RightKneeNeededCurrent = (RightKneeTorque/TORQUE2CURRENT)*1000;
LeftKneeNeededCurrent = (RightKneeTorque/TORQUE2CURRENT)*1000;

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
  CapperFloat(PWMRightKnee, 255);
  CapperFloat(PWMLeftKnee, 255);

  /*if (angle < 0.2 || angle > (2 * PI - 0.2))
    PWM = 0;*/

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

void Motor::sonarRead()
{
  int RightTrigPin = TRIG_PIN_DROIT;
  int LeftTrigPin = TRIG_PIN_GAUCHE;
  int RightEchoPin = ECHO_PIN_DROIT;
  int LeftEchoPin = ECHO_PIN_GAUCHE;

  digitalWrite(RightTrigPin, LOW);
  digitalWrite(LeftTrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(RightTrigPin, HIGH);
  digitalWrite(LeftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(RightTrigPin, LOW);
  digitalWrite(LeftTrigPin, LOW);
  

  float errorRight = 0;
  float errorLeft = 0;

  RightDuration = pulseIn(RightEchoPin, HIGH);
  LeftDuration = pulseIn(LeftEchoPin, HIGH);


  RightCm = (RightDuration / 2) / 29.1;
  LeftCm = (LeftDuration / 2) / 29.1;

  // Serial.print("Dist: ");
  // Serial.println(cm);

//Determining the current SonarState for each sensor

//Here, right sensor is examined 
  if (RightSonarState == false)
  {
    for (int i = 0; i < iteration; i++)
      if (RightCm < height)
        errorRight += 1;
    errorRight = errorRight / iteration;
    if (errorRight <= 0.2)
      RightSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (RightCm > height)
        errorRight += 1;
    errorRight = errorRight / iteration;
    if (errorRight <= 0.2)
      RightSonarState = false;
  }

//Here, left sensor is examined
   if (LeftSonarState == false)
  {
    for (int i = 0; i < iteration; i++)
      if (LeftCm < height)
        errorLeft += 1;
    errorLeft = errorLeft / iteration;
    if (errorLeft <= 0.2)
      LeftSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (LeftCm > height)
        errorLeft += 1;
    errorLeft = errorLeft / iteration;
    if (errorLeft <= 0.2)
      LeftSonarState = false;
  }


  /*if (state == false) //If the result of state is false we just turn it to true and the other way around?
  {
    // Serial.println("Sol");
    return true;
  }
  else
  {
    // Serial.println("      Air");
    return false;
  }
*/
}

void Motor::setRelais(int ID, bool state)
{
  if (ID == RELAIS_GENOU_GAUCHE)
  {
    if (state == true)
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, LOW);
  }
  else if (ID == RELAIS_GENOU_DROIT)
  {
    if (state == true)
      digitalWrite(RELAIS_PIN_GENOU_DROIT, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_DROIT, LOW);
  }
  else if (ID == RELAIS_HANCHE_GAUCHE)
  {
    if (state == true)
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, LOW);
  }
  else if (ID == RELAIS_HANCHE_DROITE)
  {
    if (state == true)
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, LOW);
  }
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
void Motor::testMotor()
{
  // Test Motor
  Serial.println("Test Motor Gauche----------");
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, 100);
  delay(300);
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, 0);
  delay(1000);
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, -100);
  delay(300);
  motorSetSpeed(MOTEUR_GENOU_GAUCHE, 0);
  delay(1000);
  Serial.println("Test Motor Droit----------");
  motorSetSpeed(MOTEUR_GENOU_DROIT, 100);
  delay(300);
  motorSetSpeed(MOTEUR_GENOU_DROIT, 0);
  delay(1000);
  motorSetSpeed(MOTEUR_GENOU_DROIT, -100);
  delay(300);
  motorSetSpeed(MOTEUR_GENOU_DROIT, 0);
  delay(1000);
}

void Motor::setAngle(enumIMU imuType, float val)
{
  switch (imuType)
  {
    case enumIMU::HipL:
        RightHipAngle = val;
        break;
    case enumIMU::KneeL:
        RightKneeAngle = val;
        break;
    case enumIMU::HipR:
        LeftHipAngle = val;
        break;
    case enumIMU::KneeR:
        LeftKneeAngle = val;
        break;
    default:
        break;
  }
}