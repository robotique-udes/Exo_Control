#include <Arduino.h>
#include <HardwareSerial.h>
#include "enums.h"
#include "pinExtender.h"
#include "quadratureEncoder.h"
#include "enums.h"
#include "touchScreen.h"
#include <SPI.h>
#include <SD.h>
#include <Nextion.h>
#include <string.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h"
#include "callbackSetup.h"
#include "dataCore.h"
#include "bnoHandler.h"
#include "proxiHandler.h"

Test tester;
Relay relais;
Motor *motor;
// Handler must be a pointer because Wire needs to be instanciated
BnoHandler *bnoHandler;
QuadratureEncoder encoder;
TouchScreen &screen = TouchScreen::getInstance();
DataCore &settings = DataCore::getInstance();
ProxiHandler proxiHandler;


void updateAngles(bool angleSource);

//===============================================================================================================
//===================================================(SETUP)=====================================================
//===============================================================================================================

void setup()
{
  Serial.begin(115200);
  nexInit();

  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  // IMU setup
  Wire.setPins(MAIN_I2C_SDA, MAIN_I2C_SCL);
  Wire.begin();

  // Setup devices using I2C
  bnoHandler = new BnoHandler();
  motor = new Motor();

  pinExtender.begin();
  QuadratureEncoder::begin();
  tester.setMotor(motor);
  tester.setEncoder(&encoder);
  tester.setProxiHandler(&proxiHandler);
  pwmPinExtender.resetDevices();
  pwmPinExtender.init();

  motor->setPins();
  relais.setPins();

  relais.setAllRelais(OFF);

  // Setup HMI
  setupCallbacks();
  // imu01.IMUSetup(); // Comment if no IMU are in use
  // imu01.wifiSetup(); // Comment if not using wifi com
}

void loop()
{

  //--------------Test BLOC----------------

  // motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);
  //tester.testRelay();
  // tester.keyboardCommand();

  //--------------LOGIC BLOC---------------
  screen.update();
  updateAngles(true);
  // motor->sonarRead(); //Ne pas décommenter, remplace par HMI
  motor->neededTorque();
  
  // motor->neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor->readCurrent(); Ne pas décommenter, pas utile sans PID
  motor->PIDCurrentPrealable();
  motor->sendCommand();

  //--------------PRINTING BLOC-------------
  // Serial.print(motor->getPower());
  // bnoHandler->printBNOs(0, 4);
  // motor->printProxim();
  // motor->printPMW();
  // motor->printTorque();
  //delay(200);

  
}

void updateAngles(bool angleSource)
{
  if(angleSource){
    //Fetch angles from IMUs
    bnoHandler->requestData();
    motor->setAngle(EnumBnoPosition::HIP_R, bnoHandler->getValAngle(EnumBnoPosition::HIP_R));
    motor->setAngle(EnumBnoPosition::HIP_L, bnoHandler->getValAngle(EnumBnoPosition::HIP_L));
    motor->setAngle(EnumBnoPosition::KNEE_R, bnoHandler->getValAngle(EnumBnoPosition::KNEE_R));
    motor->setAngle(EnumBnoPosition::KNEE_L, bnoHandler->getValAngle(EnumBnoPosition::KNEE_L));
  }
  else{
    //Fetch angles from ENCODERs
    motor->setAngle(EnumBnoPosition::HIP_R, encoder.getPositionAngle(EnumMotorPosition::HIP_R));
    motor->setAngle(EnumBnoPosition::HIP_L, encoder.getPositionAngle(EnumMotorPosition::HIP_L));
    motor->setAngle(EnumBnoPosition::KNEE_R, encoder.getPositionAngle(EnumMotorPosition::KNEE_R));
    motor->setAngle(EnumBnoPosition::KNEE_L, encoder.getPositionAngle(EnumMotorPosition::KNEE_L));
  }
}