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


void hmiLoop(void * pvParameters)
{
  hmi.begin("BioGenius");
  while(true)
  {
    hmi.update();
    delay(100);
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

  motorHandler.initializeMotors();
}

void loop() {

  delay(1000);
  return;
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
  data.backAngle = 0.0;
  data.groundedL = grounded.isLeftGrounded;
  data.groundedR = grounded.isRightGrounded;

  float torque[NB_MOTORS] = {0};
  logic.calculateTorque(data, torque);

  motorHandler.Update(torque);

  delay(100);
}