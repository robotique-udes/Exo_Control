#include <Arduino.h>
// #include <Servo.h>
// #include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
// #include "touchScreen.h"
#include <HardwareSerial.h>

HardwareSerial SerialPort(2);

#define neutral 0
#define auto 1
#define manuel 2

// Screen ecran;
Motor motor;
Test test01;


void setup()
{

  Serial.begin(9600);
  // Serial2.begin(9600, SERIAL_8N1, RX_HMI, TX_HMI);
  // nexInit();

  motor.setPins();


  Serial.println("Ini motor exo----------");
}

void loop()
{
  motor.testMotor();
  motor.testRelais();

  // ecran.nextLoop();

  // Set PWM
  /*motor.setAngle(Count_pulses);
  motor.neededTorque();
  motor.motorSetSpeed(motor.neededCurrent());
  motor.printData(Count_pulses);*/
  // delay(5);
}
