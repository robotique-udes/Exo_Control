#include "motorControl.h"

ExponentialFilter<float> FilteredCurrent(7, 0);

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


bool Motor::stopMotor(int cap) {

  //Serial.println(digitalRead(31));

  //Serial.print(" ");

  // Serial.println(digitalRead(34));

  /*if(Bouton1 == 0){

   digitalWrite(IN1 , HIGH);

   digitalWrite(IN2 , LOW);

   analogWrite(EN , 120);

  }
  else if(Bouton2 == 0){

   digitalWrite(IN2 , HIGH);

   digitalWrite(IN1 , LOW);

   analogWrite(EN , 120);

  }*/

  if(Acurrent >1300){

    analogWrite(EN , 0);
    Serial.println("loop 2");

  }

  else{

      digitalWrite(IN2 , HIGH);
      digitalWrite(IN1 , LOW);
      analogWrite(EN , 55);
      Acurrent = FilteredCurrent.Current();
      Serial.println("------loop 1");

  }
  

  CTval = analogRead(CT);
  current = ((CTval * 27.0) / 1023.0) * 1000.0;
  FilteredCurrent.Filter(current);
  Serial.println(FilteredCurrent.Current());

return true;
}

void Motor::motorSetSpeed(int val)
{
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

void Motor::motorOn()
{
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    analogWrite(EN , 150);
}
void Motor::motorOff()
{
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    analogWrite(EN , 0);
}
float Motor::ReadCurrent()
{
    CTval = analogRead(CT);
    current = ((CTval * 27.0) / 1023.0) * 1000.0;
    FilteredCurrent.Filter(current);
    return FilteredCurrent.Current()*Rotation;
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
    e = CourantSouhaite - ReadCurrent();
   
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
        Serial.print(" Angle: ");
    Serial.print(angle);
    Serial.print("Courant RESSORT: ");
    Serial.print(I_ressort);
    Serial.print(" Courant GRAVITE: ");
    Serial.print(I_gravite);
    Serial.print(" derivate: ");
    Serial.print(derivative);
    Serial.print(" Courant actuel: ");
    Serial.print(ReadCurrent());
    Serial.print(" Count_pulses: ");
    Serial.print(Count_pulses);
    Serial.print(" PWM: ");
    Serial.println(PWM);
}