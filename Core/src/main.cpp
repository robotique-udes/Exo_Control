#include <Arduino.h>
#include <Wire.h>
#include "bnoHandler.h"
#include "logic.h"
#include "motorHandler.h"
#include "hmi/HMI_Comm.h"

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
  pinMode(CAN_TERMINAL_PIN, OUTPUT);
  digitalWrite(CAN_TERMINAL_PIN, HIGH);

  if(ESP32Can.begin(ESP32Can.convertSpeed(1000), CAN_TX, CAN_RX, 5, 5)) {
    Serial.println("CAN bus started!!!");
  } else {
    Serial.println("CAN bus failed!");
  }
  delay(3000);

  motorHandler.initializeMotors();
}

void loop() {

  bnoHandler.requestData();

  Serial.println("---- Connected BNO Data ----");
  bnoHandler.printConnectedBNOsData(0, 5);
  Serial.println();


  angleOutput_t angles;
  groundedOutput_t grounded;
  angles = bnoHandler.getAngle();
  grounded = bnoHandler.getGroundedState();

  
  RequiredData data;
  data.hipAngleL = angles.hipLeft;
  data.hipAngleR = angles.hipRight;
  data.kneeAngleL =angles.KneeLeft;
  data.kneeAngleR =angles.KneeRight;
  data.backAngle = angles.back;
  data.groundedL = grounded.isLeftGrounded;
  data.groundedR = grounded.isRightGrounded;

  //data.backAngle = 0.0;
  //data.hipAngleL = 0.0;
  //data.hipAngleR = 0.0;

  float torque[NB_MOTORS] = {0};
  logic.calculateTorque(data, torque);
  motorHandler.Update(torque);

  delay(100);
}