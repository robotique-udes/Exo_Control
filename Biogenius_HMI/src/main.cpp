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

  pinMode(TRIG_PIN_GAUCHE, OUTPUT);
  pinMode(ECHO_PIN_GAUCHE, INPUT);
  pinMode(TRIG_PIN_DROIT, OUTPUT);
  pinMode(ECHO_PIN_DROIT, INPUT);

  pinMode(RELAIS_PIN_GENOU_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_GENOU_DROIT, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_GAUCHE, OUTPUT);
  pinMode(RELAIS_PIN_HANCHE_DROITE, OUTPUT);

  //MotorSetPins();

  Serial.println("Ini motor exo----------");
}

void loop()
{

  //Test Relais
  Serial.print("Relais 1: ");
  motor.setRelais(RELAIS_GENOU_GAUCHE, true);
  delay(1000);
  motor.setRelais(RELAIS_GENOU_GAUCHE, false);
  Serial.print("Relais 2: ");
  motor.setRelais(RELAIS_GENOU_DROIT, true);
  delay(1000);
  motor.setRelais(RELAIS_GENOU_DROIT, false);
  Serial.print("Relais 3: ");
  motor.setRelais(RELAIS_HANCHE_GAUCHE, true);
  delay(1000);
  motor.setRelais(RELAIS_HANCHE_GAUCHE, false);
  Serial.print("Relais 4: ");
  motor.setRelais(RELAIS_HANCHE_DROITE, true);
  delay(1000);
  motor.setRelais(RELAIS_HANCHE_DROITE, false);



  

  delay(350);
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
  // pinMode(D1_IN1_B, OUTPUT);
  // pinMode(D1_IN2_B, OUTPUT);
  // pinMode(D1_EN_B, OUTPUT);
  // pinMode(D1_CT_B, INPUT);
  pinMode(D2_IN1_A, OUTPUT);
  pinMode(D2_IN2_A, OUTPUT);
  pinMode(D2_EN_A, OUTPUT);
  pinMode(D2_CT_A, INPUT);
  // pinMode(D2_IN1_B, OUTPUT);
  // pinMode(D2_IN2_B, OUTPUT);
  // pinMode(D2_EN_B, OUTPUT);
  // pinMode(D2_CT_B, INPUT);

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