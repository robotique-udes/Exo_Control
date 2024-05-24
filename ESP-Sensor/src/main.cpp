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

Test tester;
Relay relais;
Motor *motor;
// Handler must be a pointer because Wire needs to be instanciated
IMU *imuHandler;
QuadratureEncoder encoder;
TouchScreen &screen = TouchScreen::getInstance();
ExoSettings &settings = ExoSettings::getInstance();

void updateAngles(int angleSource);

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
  tester.keyboardCommand();

  //--------------LOGIC BLOC---------------
  //screen.update();
  // updateAngles(FROM_IMU);
  // imuHandler->printBNOs(0, 4);
  // motor->sonarRead(); //Ne pas décommenter, remplace par HMI
  // motor->neededTorque();
  // motor->neededCurrent(); Ne pas décommenter, pas utile sans PID
  // motor->readCurrent(); Ne pas décommenter, pas utile sans PID
  // motor->PIDCurrentPrealable();
  // motor->sendCommand();

  //--------------PRINTING BLOC-------------
  // Serial.print(motor->getPower());
  motor->printProxim();
  //motor->printTorque();
  // Serial.println("loop");
  // delay(500);
  Serial.println(" ");
}

void updateAngles(int angleSource)
{
  switch (angleSource)
  {
  case (FROM_ENCODER):
    // Add encoder logic to fecth and send angles
    motor->setAngle(enumIMU::HIP_R, encoder.getPositionAngleRad(QuadratureEncoder::HAN_DRO));
    motor->setAngle(enumIMU::HIP_L, encoder.getPositionAngleRad(QuadratureEncoder::HAN_GAU));
    motor->setAngle(enumIMU::KNEE_R, encoder.getPositionAngleRad(QuadratureEncoder::GEN_DRO));
    motor->setAngle(enumIMU::KNEE_L, encoder.getPositionAngleRad(QuadratureEncoder::GEN_GAU));
    Serial.print("HipR: ");
    Serial.print(motor->getAngle(enumIMU::HIP_R));
    Serial.print("  HipL: ");
    Serial.print(motor->getAngle(enumIMU::HIP_L));
    Serial.print("  KneeR: ");
    Serial.print(motor->getAngle(enumIMU::KNEE_R));
    Serial.print("  KneeL: ");
    Serial.println(motor->getAngle(enumIMU::KNEE_L));
    break;
    case(FROM_IMU):
      //Need change to 085
      imuHandler->requestData();
      motor->setAngle(enumIMU::HIP_R, imuHandler->getValAngle(enumIMU::HIP_R));
      motor->setAngle(enumIMU::HIP_L, imuHandler->getValAngle(enumIMU::HIP_L));
      motor->setAngle(enumIMU::KNEE_R, imuHandler->getValAngle(enumIMU::KNEE_R));
      motor->setAngle(enumIMU::KNEE_L, imuHandler->getValAngle(enumIMU::KNEE_L));
      break;
  default:
    Serial.print("Angle source not recognized");
    break;
  }
}