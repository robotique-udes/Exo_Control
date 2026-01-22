#include <Arduino.h>
//#include <HardwareSerial.h>
#include "enums.h"
//#include "pinExtender.h"
//#include "quadratureEncoder.h"
#include "enums.h"
//#include "touchScreen.h"
//#include <SPI.h>
//#include <SD.h>
// #include <Nextion.h>
#include <string.h>
#include "motor.h"
#include "motorV2.h"
#include "motorV3.h"
//#include "test.h"
//#include "touchScreen.h"
//#include "callbackSetup.h"
#include "dataCore.h"
#include "bnoHandler.h"
#include "logic.h"

#define uart_tx 43
#define uart_rx 44
#define TX_CAN 5
#define RX_CAN 4



//Logic logicHandler;
//Test tester;
//Relay relais;
//Motor motor;
// Handler must be a pointer because Wire needs to be instanciated
//BnoHandler *bnoHandler;
//QuadratureEncoder encoder;
//TouchScreen &screen = TouchScreen::getInstance();
//DataCore &settings = DataCore::getInstance();
MotorV3 testMoteurV3_2;

  float torque = 0;


//===============================================================================================================
//===================================================(SETUP)=====================================================
//===============================================================================================================

void setup()
{
  Serial1.begin(115200, SERIAL_8N1, uart_rx, uart_tx); // for mobo
  //Serial1.begin(115200); // for proto
  //Serial1.println("Etarting...");

  
  if(ESP32Can.begin(ESP32Can.convertSpeed(1000), TX_CAN, RX_CAN, 5, 5)) {
    Serial1.println("CAN bus started!!!");
  } else {
    Serial1.println("CAN bus failed!");
  }

  
  
  //Serial12.begin(9600, Serial1_8N1, 16, 17);

  // IMU setup
  //Wire.setPins(MAIN_I2C_SDA, MAIN_I2C_SCL);
  //Wire.begin();

  // Setup devices using I2C
  //bnoHandler = new BnoHandler();
  //pinExtender.begin();
  //QuadratureEncoder::begin();
  //tester.setMotor(&motor);
  //tester.setEncoder(&encoder);
  //tester.setBNOHandler(bnoHandler);
  //tester.setLogic(&logicHandler);
  //pwmPinExtender.resetDevices();
  //pwmPinExtender.init();

  //motor.setPins();
  //relais.setPins();

  //relais.setAllRelay(OFF);

  //setupCallbacks();
  testMoteurV3_2.setMotorId(0x802);
  testMoteurV3_2.setMotorCorrection(1, 0); 
}

void loop()
{
  //---------------Test multiple motors----------------
  char rc;
  rc = Serial1.read();

  if(rc == 'a') {
    Serial1.print("a  :");
    torque -= 0.5;
    Serial1.println(torque);
  }

  if(rc == 'd') {
    Serial1.print("d  :");
    torque += 0.5;
    Serial1.println(torque,HEX);
  }

  if(rc == 's') {
    Serial1.print("s  :");
    torque = 0;
    Serial1.println(torque);
  }

  testMoteurV3_2.sendCommand(TORQUE, torque);
  






  


  
  















  //--------------Test BLOC----------------

  // motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 4000);
  // motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 4000);
  // tester.testRelay();
  //tester.keyboardCommand();


  //--------------LOGIC BLOC---------------
  // encoder.read();
  //bnoHandler->read();
  // screen.update();
  //logicHandler.Update();
  // motor.write();

  //--------------PRINTING BLOC-------------
  // bnoHandler->printBNOsStatus(0,4);
  //bnoHandler->printGroundState();
  // logicHandler.printTorque();
  // logicHandler.IntegralPowerConsumption();
  
  //Serial1.println("");
  
}
