#include "motorControl.h"


Motor::Motor()
{}

Motor::~Motor()
{}

//Servo MonServo;



bool Motor::stopMotor(int cap) {

  //Serial.println(digitalRead(31));

  //Serial.print(" ");

  // Serial.println(digitalRead(34));

  int Bouton1 = digitalRead(31);

  int Bouton2 = digitalRead(34);



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

}