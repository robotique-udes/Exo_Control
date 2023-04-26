#include <Arduino.h>
//#include <Servo.h>
//#include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
//#include "touchScreen.h"
#include <HardwareSerial.h>

//SoftwareSerial nextion(2, 3);
HardwareSerial SerialPort(2);

#define neutral 0
#define auto 1
#define manuel 2

//Screen ecran;
Motor motor;
Test test01;

void MotorSetPins();

void setup()
{

  Serial.begin(9600);
  //Serial2.begin(9600, SERIAL_8N1, RX_HMI, TX_HMI);
  //nexInit();

  MotorSetPins();

  //MotorSetPins();

  Serial.println("Ini motor exo----------");
}

void loop()
{





  // ecran.nextLoop();

  // Set PWM
  /*motor.setAngle(Count_pulses);
  motor.neededTorque();
  motor.motorSetSpeed(motor.neededCurrent());
  motor.printData(Count_pulses);*/
  // delay(5);
}

void MotorSetPins()
{
  //PINS MOTEURS
  pinMode(D1_IN1_A, OUTPUT);
  pinMode(D1_IN2_A, OUTPUT);
  pinMode(D1_EN_A, OUTPUT);
  pinMode(D1_CT_A, INPUT);
  pinMode(D2_IN1_A, OUTPUT);
  pinMode(D2_IN2_A, OUTPUT);
  pinMode(D2_EN_A, OUTPUT);
  pinMode(D2_CT_A, INPUT);

  //PINS RELAIS
  pinMode(RELAIS_PIN_GENOU_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_GENOU_DROIT, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_DROITE, OUTPUT);

  //PINS SONAR
  pinMode(TRIG_PIN_GAUCHE, OUTPUT);
  pinMode(ECHO_PIN_GAUCHE, INPUT);
  pinMode(TRIG_PIN_DROIT, OUTPUT);
  pinMode(ECHO_PIN_DROIT, INPUT);
}