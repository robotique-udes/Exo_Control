#include <Arduino.h>
#include <HardwareSerial.h>

#include "imu.h"
#include "enum.h"
#include <PinExtender.h>
#include <QuadratureEncoder.h>
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
QuadratureEncoder encoder;
TouchScreen& screen = TouchScreen::getInstance();

HardwareSerial ESP32Serial1(1);
HardwareSerial SerialPort(2);

void updateAngles();
void keyboardCommand();

//===============================================================================================================
//===================================================(SETUP)=====================================================
//===============================================================================================================

void setup()
{
  nexInit();
  Serial.begin(115200);
  Wire.begin();
  pinExtender.begin();
  QuadratureEncoder::begin(); 

  pwmPinExtender.resetDevices();
  pwmPinExtender.init();

  motor.setPins();
  relais.setPins();
  Serial.println("Ini motor exo----------");
 
  relais.setAllRelais(OFF);

  //--------------Test BLOC----------------

  // Setup HMI
  setupCallbacks();
  //imu01.IMUSetup(); // Comment if no IMU are in use
  // imu01.wifiSetup(); // Comment if not using wifi com
}

void loop()
{
  //--------------Test BLOC----------------
  // delay(400);
  Serial.print("Position Hanche Droite: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_DRO));
  Serial.print("Position Hanche Gauche: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_GAU));
  Serial.print("Position Genou Droit: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_DRO));
  Serial.print("Position Genou Gauche: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_GAU));

  // motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);

  // --------------TEST BLOC computer commands----------------


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
// Serial.println("loop");
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
  motor.setAngle(enumIMU::HipR, imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL, imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR, imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL, imu01.getValAngle(enumIMU::KneeL));
}

void keyboardCommand()
{
  if (Serial.available() > 0)
  {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // Check the value of the incoming byte and print the corresponding message
    if (incomingByte == 'w')
    {
      Serial.println("1000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 1000);
    }
    else if (incomingByte == 's')
    {
      Serial.println("1000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, -1000);
    }
    else if (incomingByte == 'e')
    {
      Serial.println("2000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 2000);
    }
    else if (incomingByte == 'd')
    {
      Serial.println("2000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, -2000);
    }
    else if (incomingByte == 'r')
    {
      Serial.println("3000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 3000);
    }
    else if (incomingByte == 'f')
    {
      Serial.println("3000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, -3000);
    }
    else if (incomingByte == 't')
    {
      Serial.println("4000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
    }
    else if (incomingByte == 'g')
    {
      Serial.println("4000");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, -4000);
    }
    else if (incomingByte == ' ')
    {
      Serial.println("stop");
      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 0);
    }
  }
}