//test commit

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
#include "SensorOPT3101.h"

//Motor motor;
// Test test01;
//Imu imu01;
// Screen ecran;
//Motor* Screen::motor = motor;

// HardwareSerial ESP32Serial1(1);
// HardwareSerial SerialPort(2);
// SensorOPT3101 distanceSensor(256);

//void updateAngles();
//void sendPWM();

  //===============================================================================================================
  //===================================================(SETUP)=====================================================
  //===============================================================================================================

void setup()
{
  // nexInit();
  Serial.begin(115200);
  // ESP32Serial1.begin(9600, SERIAL_8N1, 19, 18);
  // Serial2.begin(9600, SERIAL_8N1, 16, 17); 


  Serial.println("Ini motor exo----------");
  //SensorOPT3101 distanceSensor(256,26,27);
  // motor.setPins();
  // imu01.IMUSetup();
  // motor.setAllRelais(OFF);

  // distanceSensor.SetTriggerDistance();
  // imu01.wifiSetup();
}


void loop()
{
  //--------------Test BLOC----------------
  //delay(500);



  //--------------LOGIC BLOC---------------
  //delay(100);
  //ecran.nextLoop();

  //updateAngles(); 
  // motor.sonarRead(); Ne pas décommenter, remplace par HMI
  //motor.neededTorque();
  // motor.neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor.readCurrent(); Ne pas décommenter, pas utile sans PID
  //motor.PIDCurrentPrealable();
  //sendPWM();

  //--------------PRINTING BLOC-------------
  //Serial.print(motor.getPower());
  //motor.printSonar();
  //motor.printTorque();
  // Serial.println();
  // imu01.printAngles();
  SensorOPT3101 distanceSensor(256,26,27);
  distanceSensor.SetTriggerDistance();

  while (true){
    Serial.print("on the ground: ");
    Serial.println(distanceSensor.IsOnTheGround());
    distanceSensor.printDistance();
    Serial.println("");
  }
  
}

void updateAngles()
{
  //imu01.getAngles();
  // motor.setAngle(enumIMU::HipR,imu01.getValAngle(enumIMU::HipR));
  // motor.setAngle(enumIMU::HipL,imu01.getValAngle(enumIMU::HipL));
  // motor.setAngle(enumIMU::KneeR,imu01.getValAngle(enumIMU::KneeR));
  // motor.setAngle(enumIMU::KneeL,imu01.getValAngle(enumIMU::KneeL));
}

void sendPWM()
{
  // if (motor.PWMRightKnee > 0)
  //   motor.PWMRightKnee += 110000; //Rien-In1-In2-EN
  // else
  // {
  //   motor.PWMRightKnee = -motor.PWMRightKnee;
  //   motor.PWMRightKnee += 101000; //Rien-In1-In2-EN
  // }

  // if (motor.PWMLeftKnee > 0)
  //   motor.PWMLeftKnee += 110000; //Rien-In1-In2-EN
  // else
  // {
  //   motor.PWMLeftKnee = -motor.PWMLeftKnee;
  //   motor.PWMLeftKnee += 101000; //Rien-In1-In2-EN
  // }

  // if (motor.PWMRightHip > 0)
  //   motor.PWMRightHip += 101000; //Rien-In1-In2-EN
  // else
  // {
  //   motor.PWMRightHip = -motor.PWMRightHip;
  //   motor.PWMRightHip += 110000; //Rien-In1-In2-EN
  // }

  // if (motor.PWMLeftHip > 0)
  //   motor.PWMLeftHip += 101000; //Rien-In1-In2-EN
  // else
  // {
  //   motor.PWMLeftHip = -motor.PWMLeftHip;
  //   motor.PWMLeftHip += 110000; //Rien-In1-In2-EN
  // }


  // std::string msg = std::to_string(motor.PWMRightKnee);
  // std::string msg2 = std::to_string(motor.PWMLeftKnee);
  // std::string msg3 = std::to_string(motor.PWMRightHip);
  // std::string msg4 = std::to_string(motor.PWMLeftHip);

  
  
  // std::string msg5 = msg + msg2 + msg3 + msg4 + "\n";
  // // ESP32Serial1.write(msg5.c_str());
  // // Serial.print(msg5.c_str());
  // motor.PWMRightKnee = 0;
  // motor.PWMLeftKnee = 0;
  // motor.PWMRightHip = 0;
  // motor.PWMLeftHip = 0;


}