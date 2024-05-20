
#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>
#include "motorControl.h"
#include "test.h"
#include "imu.h" 
#include "touchScreen.h"
#include "callbackSetup.h"
#include "imu.h" 
#include "enumIMU.h"

Test tester;
Relay relais;
Motor motor;
Imu imu01;
TouchScreen& screen = TouchScreen::getInstance();

HardwareSerial ESP32Serial1(1);
HardwareSerial SerialPort(2);

void updateAngles();
void sendPWM();

  //===============================================================================================================
  //===================================================(SETUP)=====================================================
  //===============================================================================================================

void setup()
{
  relais.setPins();
  nexInit();
  Serial.begin(9600);
  ESP32Serial1.begin(9600, SERIAL_8N1, 19, 18);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  Serial.println("Ini motor exo----------");
  motor.setPins();

  relais.setAllRelais(OFF);

  // Setup HMI
  setupCallbacks();
  imu01.IMUSetup(); // Comment if no IMU are in use
  // imu01.wifiSetup(); // Comment if not using wifi com
}
void loop()
{
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
  Serial.println("");

  // Obligatoire pour le HMI
  screen.update();
}

void updateAngles()
{
  imu01.getAngles();
  motor.setAngle(enumIMU::HipR,imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL,imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR,imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL,imu01.getValAngle(enumIMU::KneeL));
}

void sendPWM()
{
  if (motor.PWMRightKnee > 0)
    motor.PWMRightKnee += 110000; //Rien-In1-In2-EN
  else
  {
    motor.PWMRightKnee = -motor.PWMRightKnee;
    motor.PWMRightKnee += 101000; //Rien-In1-In2-EN
  }

  if (motor.PWMLeftKnee > 0)
    motor.PWMLeftKnee += 110000; //Rien-In1-In2-EN
  else
  {
    motor.PWMLeftKnee = -motor.PWMLeftKnee;
    motor.PWMLeftKnee += 101000; //Rien-In1-In2-EN
  }

  if (motor.PWMRightHip > 0)
    motor.PWMRightHip += 101000; //Rien-In1-In2-EN
  else
  {
    motor.PWMRightHip = -motor.PWMRightHip;
    motor.PWMRightHip += 110000; //Rien-In1-In2-EN
  }

  if (motor.PWMLeftHip > 0)
    motor.PWMLeftHip += 101000; //Rien-In1-In2-EN
  else
  {
    motor.PWMLeftHip = -motor.PWMLeftHip;
    motor.PWMLeftHip += 110000; //Rien-In1-In2-EN
  }


  std::string msg = std::to_string(motor.PWMRightKnee);
  std::string msg2 = std::to_string(motor.PWMLeftKnee);
  std::string msg3 = std::to_string(motor.PWMRightHip);
  std::string msg4 = std::to_string(motor.PWMLeftHip);
  
  
  std::string msg5 = msg + msg2 + msg3 + msg4 + "\n";
  ESP32Serial1.write(msg5.c_str());
  Serial.print(msg5.c_str());
  motor.PWMRightKnee = 0;
  motor.PWMLeftKnee = 0;
  motor.PWMRightHip = 0;
  motor.PWMLeftHip = 0;


}