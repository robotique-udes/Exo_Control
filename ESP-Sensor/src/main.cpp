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

HardwareSerial ESP32Serial1(1);
HardwareSerial SerialPort(2);

void updateAngles();
void sendPWM();

  //===============================================================================================================
  //===================================================(SETUP)=====================================================
  //===============================================================================================================

void setup()
{
  nexInit();
  Serial.begin(9600);
  ESP32Serial1.begin(9600, SERIAL_8N1, 19, 18);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 


  Serial.println("Ini motor exo----------");
  //imu01.IMUSetup();

  motor.setPins();
  imu01.IMUSetup();
  motor.setAllRelais(ON);

  //imu01.wifiSetup();
}
void loop()
{
  //--------------Test BLOC----------------
  //delay(500);



  //--------------LOGIC BLOC---------------
  //delay(100);
  ecran.nextLoop();
  //Serial.print("Sonar height: ");
   updateAngles();
   motor.sonarRead();
   motor.neededTorque();
   motor.neededCurrent();
  // motor.readCurrent();
  //motor.PIDCurrentPrealable();
  //sendPWM();

  //--------------PRINTING BLOC-------------
   //motor.printSonar();
   //motor.printTorque();
   //imu01.printAngles();
   //Serial.println("");

  

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
    motor.PWMRightKnee += 101000; //Rien-In1-In2-EN
  else
  {
    motor.PWMRightKnee = -motor.PWMRightKnee;
    motor.PWMRightKnee += 110000; //Rien-In1-In2-EN
  }

  if (motor.PWMLeftKnee > 0)
    motor.PWMLeftKnee += 101000; //Rien-In1-In2-EN
  else
  {
    motor.PWMLeftKnee = -motor.PWMLeftKnee;
    motor.PWMLeftKnee += 110000; //Rien-In1-In2-EN
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
  Serial.println(msg5.c_str());
  motor.PWMRightKnee = 0;
  motor.PWMLeftKnee = 0;
  motor.PWMRightHip = 0;
  motor.PWMLeftHip = 0;


}