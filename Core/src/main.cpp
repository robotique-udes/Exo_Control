#include <Arduino.h>
#include <Wire.h>
#include "bnoHandler.h"
#include "logic.h"
#include "motorHandler.hpp"
#include "hmi/HMI_Comm.h"
#include "config.h"

namespace pin_config = app::config::pins;
namespace motor_config = app::config::motors;
namespace bno_config = app::config::bnos;
namespace debug = app::config::debug;

void hmiLoop(void * pvParameters);

static BnoHandler bnoHandler;
static Logic logic;
static MotorHandler motorHandler;
static HMI_Comm hmi;
SemaphoreHandle_t motorPowerMutex;


void hmiLoop(void * pvParameters)
{
  hmi.begin();
  hmi.setLogic(&logic);
  hmi.setMotorHandler(&motorHandler);
  while(true)
  {
    hmi.update();
    delay(1000);
  }
}


void setup() {
  Serial.begin(115200);

  Wire.begin();
  delay(500);

  //starts the hmi logic on the core 0
  xTaskCreatePinnedToCore(
    hmiLoop, 
    "Hmi_Task_on_Core0", 
    10000,      
    NULL,       
    1,          
    NULL,       
    0           // <--- Pinned to Core 0
  );


  Serial.println("Starting BnoHandler...");
  const bool hasConnectedBno = bnoHandler.begin();

  if (!hasConnectedBno) {
    Serial.println("No BNO connected.");
  } else {
    Serial.println("BNO setup complete. Reading connected sensors only.");
  }
  pinMode(pin_config::can_terminal, OUTPUT);
  digitalWrite(pin_config::can_terminal, HIGH);

  if(ESP32Can.begin(ESP32Can.convertSpeed(1000), pin_config::can_tx, pin_config::can_rx, 5, 5)) {
    Serial.println("CAN bus started!!!");
  } else {
    Serial.println("CAN bus failed!");
  }
  delay(3000);
}

void loop() {
  bnoHandler.requestData();

  if (debug::main)
  {
    Serial.println("---- Connected BNO Data ----");
    bnoHandler.printConnectedBNOsData(0, 5);
    Serial.println();
  }

  float angles[bno_config::amount] = {0};
  bool grounded[bno_config::nb_leg] = {true};
  bnoHandler.getAngle(angles);
  bnoHandler.getGroundedState(grounded);
  
  for (int i = 0; i < bno_config::amount; i++)
    Serial.println(angles[i]);

  Serial.println(grounded[0]);
  Serial.println(grounded[1]);

  float torque[motor_config::amount] = {0};
  logic.calculateTorque(angles, grounded, torque);

/*   float val = 15;
  float test[4] = {val, val, val, val}; */
  motorHandler.update({torque[0], torque[1], torque[2], torque[3]});

  delay(100);
}