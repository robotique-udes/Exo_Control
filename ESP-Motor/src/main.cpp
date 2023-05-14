#include <Arduino.h>
//#include <Servo.h>
//#include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h"
#include <HardwareSerial.h>

//SoftwareSerial nextion(2, 3);
HardwareSerial SerialPort(2);

#define neutral 0
#define auto 1
#define manuel 2

Screen ecran;
Motor motor;
Test test01;

long Count_pulses = 0;

void ReadEncoder();
void CapperFloat(float &val, float cap);

int trigPin = 26;  // Trigger
int echoPin = 27;  // Echo

void setup()
{

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  nexInit();

  pinMode(31, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(CT, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(ENCA, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Ini motor exo----------");

  attachInterrupt(digitalPinToInterrupt(ENCA), ReadEncoder, RISING);

  
}

void loop()
{

  // ecran.nextLoop();
  digitalWrite(trigPin, LOW);

 delayMicroseconds(5);

 digitalWrite(trigPin, HIGH);

 delayMicroseconds(10);

 digitalWrite(trigPin, LOW);


 pinMode(echoPin, INPUT);
 test01.testSonar();
  // Set PWM
  /*motor.setAngle(Count_pulses);
  motor.neededTorque();
  motor.motorSetSpeed(motor.neededCurrent());
  motor.printData(Count_pulses);*/
  // delay(5);
}

void ReadEncoder()
{
  int b = digitalRead(ENCB);
  if (b > 0)
  {
    Count_pulses++;
  }
  else
  {
    Count_pulses--;
  }
}
