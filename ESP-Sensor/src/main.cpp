
#include <Arduino.h>
#include "motorControl.h"
#include "test.h"
#include <HardwareSerial.h>
#include "biogeniusBno.h"
#include "biogeniusImu.h"
#include "touchScreen.h"
#include <SPI.h>
#include <SD.h>
#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>
#include "enum.h"
#include "multiplex.h"

Test tester;
Relay relais;
Motor motor;
IMU imu01;
Screen ecran;
Motor* Screen::motor = motor;
int gaming = 0;

HardwareSerial ESP32Serial1(1);
HardwareSerial SerialPort(2);

void updateAngles();
void sendPWM();

  //===============================================================================================================
  //===================================================(SETUP)=====================================================
  //===============================================================================================================

void setup()
{
  Serial.begin(115200);

  imu01 = IMU();

  imu01.begin();

}
void loop()
{
  imu01.requestData();

  if (gaming % 100) {
    imu01.getValAngle(enumIMU::Back);
  }

  //--------------Test BLOC----------------
  //delay(500);



  //--------------LOGIC BLOC---------------
  //ecran.nextLoop();
  //updateAngles(); 
  //motor.sonarRead(); //Ne pas décommenter, remplace par HMI
  //motor.neededTorque();
  // motor.neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor.readCurrent(); Ne pas décommenter, pas utile sans PID
  //motor.PIDCurrentPrealable();
  //sendPWM();

  //--------------PRINTING BLOC-------------
  //Serial.print(motor.getPower());
  //motor.printSonar();
  //motor.printTorque();
  //imu01.printAngles();
}
