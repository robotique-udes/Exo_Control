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
#include <QuadratureEncoder.h>
// #include "Wire.h"

Test tester;
Relay relais;
Motor motor;
Imu imu01;
Screen ecran;
Motor *Screen::motor = motor;
QuadratureEncoder encoder;

void updateAngles();

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

  relais.setPins();
  motor.setPins();
  relais.setAllRelais(OFF);
  // imu01.IMUSetup();
  //  imu01.wifiSetup();
  Serial.println("Ini motor exo----------");

  //--------------Test BLOC----------------
  // pinExtender.pinMode(0, OUTPUT);
  // pinExtender.pinMode(1, OUTPUT);
  // pinExtender.pinMode(2, OUTPUT);
}

void loop()
{
  //--------------Test BLOC----------------
  // delay(400);
  // Serial.print("Position Hanche Droite: ");
  // Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_DRO));
  // Serial.print("Position Hanche Gauche: ");
  // Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_GAU));
  // Serial.print("Position Genou Droit: ");
  // Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_DRO));
   Serial.print("Position Genou Gauche: ");
   Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_GAU));

  // motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);

  // --------------TEST BLOC computer commands----------------
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
}

void updateAngles()
{
  imu01.getAngles();
  motor.setAngle(enumIMU::HipR, imu01.getValAngle(enumIMU::HipR));
  motor.setAngle(enumIMU::HipL, imu01.getValAngle(enumIMU::HipL));
  motor.setAngle(enumIMU::KneeR, imu01.getValAngle(enumIMU::KneeR));
  motor.setAngle(enumIMU::KneeL, imu01.getValAngle(enumIMU::KneeL));
}