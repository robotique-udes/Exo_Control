#include "motorControl.h"

//ExponentialFilter<float> FilteredCurrent(7, 0);

Motor::Motor()
{}

Motor::~Motor()
{}

//Servo MonServo;

/*Fonctions de torque a integrer

TgSol = (((cos(angleGenoux)*LF) - 0.5*LH*cos(angleHanche))*Fgh) + (0.5*LF*cos(angleGenoux)*Fgf);
TgVide = (cos(angleGenoux)*Fgt*LG)/2;
ThSol = (0.5*LH*cos(angleHanche)*Fgh);
ThVide = (cos(angleHanche)*LH*Fgf)/2 + (cos(angleHanche)*LF + (cos(angleGenoux)*LG)/2)*Fgt;
*/




void Motor::motorSetSpeed(int ID, int val)
{

  int IN1;
  int IN2;
  int EN;

  if(ID == MOTEUR_GENOU_GAUCHE)
  {
    IN1 = D1_IN1_A;
    IN2 = D1_IN2_A;
    EN = D1_EN_A;
  }
  else if(ID == MOTEUR_GENOU_DROIT)
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

  if(val >= 0)
  {
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    Rotation = ClockWise;
  }
  else if(val < 0)
  {
    digitalWrite(IN2 , LOW);
    digitalWrite(IN1 , HIGH);
    Rotation = CounterClockWise;
    val = -val;
  }

    analogWrite(EN , val);
}


float Motor::ReadCurrent(int ID)
{
  int CT;

  if(ID == MOTEUR_GENOU_GAUCHE)
    CT = D1_CT_A;
  else if(ID == MOTEUR_GENOU_DROIT)
    CT = D2_CT_A;
  // else if(ID == MOTEUR_HANCHE_GAUCHE)
  //   CT = D1_CT_B;
  // else if(ID == MOTEUR_HANCHE_DROITE)
  //   CT = D2_CT_B;

    CTval = analogRead(CT);
    current = ((CTval * 27.0) / 1023.0) * 1000.0;
    //FilteredCurrent.Filter(current);
    //return FilteredCurrent.Current()*Rotation;
    return current;
}

void Motor::setAngle( long Count_pulses)
{
  angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
  if(angle > 2*PI)
  {
    Count_pulses -= PULSE_PAR_TOUR;
    angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
    }
  else if (angle < 0)
  {
    Count_pulses +=  PULSE_PAR_TOUR;
    angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
  }
}

void Motor::CapperFloat(float &val, float max)
{
  if(val > max)
    val = max;
  else if(val < -max)
    val = -max;
}

void Motor::neededTorque()
{
    //Trouver valeur du courant pour gravité
    T_gravite = DIST_CM * MASSE * 9.81 * sin(angle);
    CourantSouhaite = T_gravite / TORQUE2CURRENT * 1000;
}

float Motor::neededCurrent()
{
      //PID for current 
    e = CourantSouhaite - ReadCurrent(MOTEUR_GENOU_DROIT);
   
    integral = integral + e;
    derivative = e - previous_error;
    CapperFloat(e, 5);
    //CapperFloat(derivative, 50);
    PWM += KP*e + KI*integral + KD*derivative;
    previous_error = e;

    CapperFloat(PWM, 255);
    if(angle<0.2 || angle>(2*PI-0.2))
      PWM = 0;
    
    return PWM;

}

void Motor::printData(long Count_pulses)
{
    // Serial.print(" Angle: ");
    // Serial.print(angle);
    Serial.print(" Courant GRAVITE: ");
    Serial.print(CourantSouhaite);
    // Serial.print(" derivate: ");
    // Serial.print(derivative);
    Serial.print(" Courant actuel: ");
    Serial.print(ReadCurrent(MOTEUR_GENOU_DROIT));
    // Serial.print(" Count_pulses: ");
    // Serial.print(Count_pulses);
    Serial.print(" PWM: ");
    Serial.println(PWM);
}

bool Motor::sonarRead(int ID)
{
  int trigPin;
  int echoPin;

  if(ID == SONAR_GAUCHE)
  {
    trigPin = TRIG_PIN_GAUCHE;
    echoPin = ECHO_PIN_GAUCHE;
  }
  else if(ID == SONAR_DROIT)
  {
    trigPin = TRIG_PIN_DROIT;
    echoPin = ECHO_PIN_DROIT;
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


    float erreur = 0;
    duration = pulseIn(echoPin, HIGH);

    cm = (duration/2) / 29.1;

    // Serial.print("Dist: ");
    // Serial.println(cm);

    inches = (duration/2) / 74;
    if (state==false)
    {
        for (int i =0; i<iteration;i++) if (cm<height) erreur+=1;
        erreur=erreur/iteration;
        if(erreur<=0.2) state=true;
    }
    else
    {
        for (int i=0; i<iteration;i++) if (cm>height) erreur+=1;
        erreur=erreur/iteration;
        if(erreur<=0.2) state=false;
    }
    if(state==false)
    {
        //Serial.println("Sol");
        return true;
    }
    else
    {
        //Serial.println("      Air");
        return false;
    }

    //Serial.print(inches);

    //Serial.print("in, ");
    // Serial.print("Dist: ");
    //Serial.println(state);
    // Serial.println(cm);

    //Serial.print("cm");
    //Serial.println();
    //delay(200);

}

void Motor::setRelais(int ID, bool state)
{
  if(ID == RELAIS_GENOU_GAUCHE)
  {
    if(state == true)
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_GAUCHE, LOW);
  }
  else if(ID == RELAIS_GENOU_DROIT)
  {
    if(state == true)
      digitalWrite(RELAIS_PIN_GENOU_DROIT, HIGH);
    else
      digitalWrite(RELAIS_PIN_GENOU_DROIT, LOW);
  }
  else if(ID == RELAIS_HANCHE_GAUCHE)
  {
    if(state == true)
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_GAUCHE, LOW);
  }
  else if(ID == RELAIS_HANCHE_DROITE)
  {
    if(state == true)
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, HIGH);
    else
      digitalWrite(RELAIS_PIN_HANCHE_DROITE, LOW);
  }
}

void Motor::testRelais()
{
 //Test Relais
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
  //Test Motor
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