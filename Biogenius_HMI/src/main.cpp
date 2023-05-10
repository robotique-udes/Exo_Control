#include <Arduino.h>
// #include <Servo.h>
// #include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
// #include "touchScreen.h"
#include <HardwareSerial.h>
#include "imu.h" 
#include "touchScreen.h"
#include <SPI.h>
//#include <SoftwareSerial.h>
#include <SD.h>
#include "imu.h" //jesus was here

#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>

#include "enum.h"

Motor motor;
Test test01;
Imu imu01;
Screen ecran;

HardwareSerial SerialPort(2);

void updateAngles();

  //===============================================================================================================
  //===================================================(SETUP)=====================================================
  //===============================================================================================================

void setup()
{
  nexInit();
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  motor.setPins();

  Serial.println("Ini motor exo----------");
  imu01.IMUSetup();
  //imu01.wifiSetup();
}

void loop()
{

  imu01.printAngles();
  delay(200);
  //nexLoop(nex_listen_list);
  ecran.nextLoop();
  updateAngles();
  //imu01.printAngles();
  //motor.testMotor();
  //motor.testRelais();
  // Set PWM
  //motor.setAngle(Count_pulses);
  //motor.neededTorque();
  //motor.motorSetSpeed(motor.neededCurrent());
  //motor.printData(Count_pulses);
  delay(5);
}

void updateAngles()
{
  motor.setAngle(enumIMU::HipR,imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL,imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR,imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL,imu01.getValAngle(enumIMU::KneeL));
}