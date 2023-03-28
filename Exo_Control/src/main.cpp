#include <Arduino.h>
#include <Servo.h>
#include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h"

SoftwareSerial nextion(2, 3);

#define neutral 0
#define auto 1
#define manuel 2





Screen ecran;
Motor motor;

long Count_pulses = 0;

void ReadEncoder();
void CapperFloat(float &val, float cap);

void setup()
{

  Serial.begin(9600);
  nexInit();

  pinMode(31, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(CT, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(ENCA, INPUT);

  Serial.println("Ini motor exo----------");

  attachInterrupt(digitalPinToInterrupt(ENCA), ReadEncoder, RISING);
}

void loop()
{
  // ecran.nextLoop();

  // Set PWM
  motor.setAngle(Count_pulses);
  motor.neededTorque();
  motor.motorSetSpeed(motor.neededCurrent());
  motor.printData(Count_pulses);
  delay(10);
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
