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
#include "motor.h"
#include "test.h"
#include "touchScreen.h"
#include "callbackSetup.h"
#include "dataCore.h"
#include "bnoHandler.h"
#include "logic.h"

Logic logicHandler;
Test tester;
Relay relais;
Motor motor;
// Handler must be a pointer because Wire needs to be instanciated
BnoHandler *bnoHandler;
QuadratureEncoder encoder;
TouchScreen &screen = TouchScreen::getInstance();
DataCore &settings = DataCore::getInstance();


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
  pinExtender.begin();
  QuadratureEncoder::begin();
  tester.setMotor(&motor);
  tester.setEncoder(&encoder);
  tester.setBNOHandler(bnoHandler);
  tester.setLogic(&logicHandler);
  pwmPinExtender.resetDevices();
  pwmPinExtender.init();

  motor.setPins();
  relais.setPins();

  relais.setAllRelay(OFF);

  setupCallbacks();
}

void loop()
{

  //--------------Test BLOC----------------

  // motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);
  // tester.testRelay();
  tester.keyboardCommand();

  //--------------LOGIC BLOC---------------
  // encoder.read();
  bnoHandler->read();
  // screen.update();
  logicHandler.Update();
  // motor.write();

  //--------------PRINTING BLOC-------------
  // bnoHandler->printBNOsStatus(0,4);
  bnoHandler->printGroundState();
  // logicHandler.printTorque();
  // logicHandler.IntegralPowerConsumption();
  
  Serial.println("");

  
}
