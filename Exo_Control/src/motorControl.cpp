#include "motorControl.h"

int IN1 = 4;

int IN2 = 3;

int EN = 2;

int CT = A11;

float CTval=0.0;

float current=0.0;

ExponentialFilter<float> FilteredCurrent(7, 0);

float Acurrent;


//Servo MonServo;



void setup() {



  Serial.begin(9600);

  pinMode(31 , INPUT_PULLUP);

  pinMode(34 , INPUT_PULLUP);

  pinMode(EN , OUTPUT);

  pinMode(IN1 , OUTPUT);

  pinMode(IN2 , OUTPUT);

  pinMode(CT, INPUT);




}



void loop() {

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