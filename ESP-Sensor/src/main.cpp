#include <Arduino.h>
#include <HardwareSerial.h>
#include "imu.h"
#include "enumIMU.h"
#include "PinExtender.h"
#include "QuadratureEncoder.h"
#include <SPI.h>
#include <SD.h>
#include <Nextion.h>
#include <string.h>
#include "motorControl.h"
#include "test.h" 
#include "touchScreen.h"
#include "callbackSetup.h"


Test tester;
Relay relais;
Motor motor;
Imu imu01;
QuadratureEncoder encoder;
TouchScreen& screen = TouchScreen::getInstance();


void updateAngles();
void keyboardCommand();



//===============================================================================================================
//===================================================(SETUP)=====================================================
//===============================================================================================================

void setup()
{
  Serial.begin(115200);
  nexInit();

  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  Wire.begin();
  pinExtender.begin();
  QuadratureEncoder::begin(); 
  tester.setMotor(&motor);
  tester.setEncoder(&encoder);
  pwmPinExtender.resetDevices();
  pwmPinExtender.init();

  motor.setPins();
  relais.setPins();
 
  relais.setAllRelais(OFF);

  //--------------Test BLOC----------------

  // Setup HMI
  setupCallbacks();
  // imu01.IMUSetup(); // Comment if no IMU are in use
  // imu01.wifiSetup(); // Comment if not using wifi com

}

void loop()
{
  //--------------Test BLOC----------------
  delay(500);
  Serial.print("Position Hanche Droite: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_DRO));
  Serial.print("Position Hanche Gauche: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::HAN_GAU));
  Serial.print("Position Genou Droit: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_DRO));
  Serial.print("Position Genou Gauche: ");
  Serial.println(encoder.getPositionPulses(QuadratureEncoder::GEN_GAU));

  Serial.print("Position Hanche Droite: ");
  Serial.println(encoder.getPositionAngle(QuadratureEncoder::HAN_DRO));
  Serial.print("Position Hanche Gauche: ");
  Serial.println(encoder.getPositionAngle(QuadratureEncoder::HAN_GAU));
  Serial.print("Position Genou Droit: ");
  Serial.println(encoder.getPositionAngle(QuadratureEncoder::GEN_DRO));
  Serial.print("Position Genou Gauche: ");
  Serial.println(encoder.getPositionAngle(QuadratureEncoder::GEN_GAU));


  // motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor.motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);

  // --------------TEST BLOC computer commands----------------
  // tester.keyboardCommand();

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

