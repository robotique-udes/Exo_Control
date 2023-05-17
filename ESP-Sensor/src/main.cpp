#include <Arduino.h>
// #include <Servo.h>
// #include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
#include <HardwareSerial.h>
#include "imu.h" 
#include "touchScreen.h"
#include <SPI.h>
//#include <SoftwareSerial.h>
#include <SD.h>
#include "imu.h" 

#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>

#include "enum.h"

Motor motor;
Test test01;
Imu imu01;
Screen ecran;
Motor* Screen::motor = motor;

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
  //imu01.IMUSetup();
  motor.setAllRelais(OFF);
  //imu01.wifiSetup();
}

void loop()
{
  //--------------LOGIC BLOC---------------
  delay(200);
  ecran.nextLoop();
   Serial.print("Height set to: ");
  Serial.println(motor.getHeight());
  //updateAngles();
  //motor.sonarRead();
  //motor.neededTorque();
  // motor.neededCurrent();
  // motor.readCurrent();
  // motor.PIDCurrent();

  //--------------PRINTING BLOC-------------
  // motor.printSonar();
  // motor.printTorque();
  // imu01.printAngles();

}

void updateAngles()
{
  imu01.getAngles();
  motor.setAngle(enumIMU::HipR,imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL,imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR,imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL,imu01.getValAngle(enumIMU::KneeL));
}