#include <Arduino.h>
#include "motorControl.h"
#include "test.h"
#include <HardwareSerial.h>
#include "imu.h"
#include "touchScreen.h"
#include <SPI.h>
#include <SD.h>
#include "imu.h"
#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>
#include "enum.h"
#include <PinExtender.h>
//#include "Wire.h"

Test tester;
Relay relais;
Motor motor;
Imu imu01;
Screen ecran;
Motor *Screen::motor = motor;

void updateAngles();

//===============================================================================================================
//===================================================(SETUP)=====================================================
//===============================================================================================================

void setup()
{
  nexInit();
  Serial.begin(115200);
  Wire.begin();

  Serial.println("pinExtender.begin();");
  pinExtender.begin(); //  Default address 0x20

  pwmPinExtender.resetDevices();
  pwmPinExtender.init();
  pwmPinExtender.setPWMFrequency(200); 
  Serial.println("pinExtender.end();");

  relais.setPins();
  motor.setPins();
  relais.setAllRelais(OFF);
  //imu01.IMUSetup();
  // imu01.wifiSetup();
  Serial.println("Ini motor exo----------");

  //--------------Test BLOC----------------
  pinExtender.pinMode(0, OUTPUT);
  pinExtender.pinMode(1, OUTPUT);
  pinExtender.pinMode(2, OUTPUT);
}

void loop()
{
  //--------------Test BLOC----------------
  delay(1000);
  // Set the pin to the opposite state to test GPIO extender
  // Is working correctly
  pinExtender.digitalWrite(0, pinExtender.digitalRead(0) ? LOW : HIGH);
  pinExtender.digitalWrite(1, pinExtender.digitalRead(1) ? LOW : HIGH);
  pinExtender.digitalWrite(2, pinExtender.digitalRead(2) ? LOW : HIGH);

  pwmPinExtender.setChannelPWM(0, 2048);
  pwmPinExtender.setChannelPWM(1, 2048);
  pwmPinExtender.setChannelPWM(2, 2048);
  pwmPinExtender.setChannelPWM(3, 2048);
  motor.testFuncSetPwnHigh();




  //--------------LOGIC BLOC---------------
  // ecran.nextLoop();
  // updateAngles();
  // motor.sonarRead(); //Ne pas décommenter, remplace par HMI
  // motor.neededTorque();
  // motor.neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor.readCurrent(); Ne pas décommenter, pas utile sans PID
  // motor.PIDCurrentPrealable();
  // motor.sendCommand();

  //--------------PRINTING BLOC-------------
  // Serial.print(motor.getPower());
  // motor.printSonar();
  // motor.printTorque();
  // imu01.printAngles();
  Serial.println("loop");
}

void updateAngles()
{
  imu01.getAngles();
  motor.setAngle(enumIMU::HipR, imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL, imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR, imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL, imu01.getValAngle(enumIMU::KneeL));
}