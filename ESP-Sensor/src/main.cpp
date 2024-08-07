#include <Arduino.h>
#include <HardwareSerial.h>
#include "enumIMU.h"
#include "PinExtender.h"
#include "QuadratureEncoder.h"
#include "biogeniusImu.h"
#include "touchScreen.h"
#include <SPI.h>
#include <SD.h>
#include <Nextion.h>
#include <string.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h"
#include "callbackSetup.h"
#include "exoSettings.h"
#include "enumAngleSource.h"
#include "wifiSimulation.h"

Test tester;
Relay relais;
WifiSimulation wifi;
Motor *motor;
// Handler must be a pointer because Wire needs to be instanciated
IMU *imuHandler;
QuadratureEncoder encoder;
TouchScreen &screen = TouchScreen::getInstance();
ExoSettings &settings = ExoSettings::getInstance();

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
  imuHandler = new IMU();
  motor = new Motor();

  pinExtender.begin();
  QuadratureEncoder::begin();
  tester.setMotor(motor);
  tester.setEncoder(&encoder);
  pwmPinExtender.resetDevices();
  pwmPinExtender.init();
  settings.init(&encoder);

  motor->setPins();
  relais.setPins();

  relais.setAllRelais(OFF);

  // Setup HMI
  setupCallbacks();

  // Setup wifi
  if (WIFI_ACTIVE)
  {
    wifi.wifiSetup();
  }
}

void loop()
{

  //--------------Test BLOC----------------

  // motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);
  // tester.testRelay();
  // tester.keyboardCommand();

  //--------------LOGIC BLOC---------------
  screen.update();
  updateAngles(settings.getAngleSource());
  // motor->sonarRead(); //Ne pas décommenter, remplace par HMI
  motor->neededTorque();

  // motor->neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor->readCurrent(); Ne pas décommenter, pas utile sans PID
  motor->PIDCurrentPrealable();
  motor->sendCommand();

  //--------------PRINTING BLOC-------------
  // Serial.print(motor->getPower());
  // imuHandler->printBNOs(0, 4);
  // motor->printProxim();
  // motor->printPMW();
  // motor->printTorque();
  // delay(200);
}

void updateAngles(bool angleSource)
{
  if (angleSource)
  {
    // Fetch angles from IMUs
    imuHandler->requestData();
    motor->setAngle(enumIMU::HIP_R, imuHandler->getValAngle(enumIMU::HIP_R));
    motor->setAngle(enumIMU::HIP_L, imuHandler->getValAngle(enumIMU::HIP_L));
    motor->setAngle(enumIMU::KNEE_R, imuHandler->getValAngle(enumIMU::KNEE_R));
    motor->setAngle(enumIMU::KNEE_L, imuHandler->getValAngle(enumIMU::KNEE_L));

    if (WIFI_ACTIVE)//ICI, ON PASSE LA VALEUR 
    {
      //--------SetAngle Y---------//
      wifi.setAngle(enumIMU::TIBIA_L, imuHandler->getValAngle(enumIMU::TIBIA_L));
      wifi.setAngle(enumIMU::TIBIA_R, imuHandler->getValAngle(enumIMU::TIBIA_R));
      wifi.setAngle(enumIMU::THIGH_L, imuHandler->getValAngle(enumIMU::THIGH_L));
      wifi.setAngle(enumIMU::THIGH_R, imuHandler->getValAngle(enumIMU::THIGH_R));
      wifi.setAngle(enumIMU::EXO_BACK, imuHandler->getValAngle(enumIMU::EXO_BACK));

      //--------SetAngle X---------//      
      wifi.setAngle(enumIMU::TIBIA_LX, imuHandler->getValAngle(enumIMU::TIBIA_LX));
      wifi.setAngle(enumIMU::TIBIA_RX, imuHandler->getValAngle(enumIMU::TIBIA_RX));
      wifi.setAngle(enumIMU::THIGH_LX, imuHandler->getValAngle(enumIMU::THIGH_LX));
      wifi.setAngle(enumIMU::THIGH_RX, imuHandler->getValAngle(enumIMU::THIGH_RX));
      wifi.setAngle(enumIMU::EXO_BACKX, imuHandler->getValAngle(enumIMU::EXO_BACKX));

	    //--------SetAngle Z---------// 
      wifi.setAngle(enumIMU::TIBIA_LZ, imuHandler->getValAngle(enumIMU::TIBIA_LZ));
      wifi.setAngle(enumIMU::TIBIA_RZ, imuHandler->getValAngle(enumIMU::TIBIA_RZ));
      wifi.setAngle(enumIMU::THIGH_LZ, imuHandler->getValAngle(enumIMU::THIGH_LZ));
      wifi.setAngle(enumIMU::THIGH_RZ, imuHandler->getValAngle(enumIMU::THIGH_RZ));
      wifi.setAngle(enumIMU::EXO_BACKZ, imuHandler->getValAngle(enumIMU::EXO_BACKZ));
    }
  }
  else
  {
    // Fetch angles from ENCODERs
    motor->setAngle(enumIMU::HIP_R, encoder.getPositionAngle(QuadratureEncoder::HAN_DRO));
    motor->setAngle(enumIMU::HIP_L, encoder.getPositionAngle(QuadratureEncoder::HAN_GAU));
    motor->setAngle(enumIMU::KNEE_R, encoder.getPositionAngle(QuadratureEncoder::GEN_DRO));
    motor->setAngle(enumIMU::KNEE_L, encoder.getPositionAngle(QuadratureEncoder::GEN_GAU));
  }
}