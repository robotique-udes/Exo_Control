#include "motorControl.h"

ExponentialFilter<float> FilteredCurrent(7, 0);

Motor::Motor()
{}

Motor::~Motor()
{}

//Servo MonServo;



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


