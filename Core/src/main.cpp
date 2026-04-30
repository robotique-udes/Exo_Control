#include <Arduino.h>
#include <Wire.h>
#include "bnoHandler.h"
#include "logic.h"

static BnoHandler bnoHandler;
static Logic logic;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  delay(500);

  Serial.println("Starting BnoHandler...");
  const bool hasConnectedBno = bnoHandler.begin();

  if (!hasConnectedBno) {
    Serial.println("No BNO connected.");
  } else {
    Serial.println("BNO setup complete. Reading connected sensors only.");
  }
}

void loop() {

  bnoHandler.requestData();

  //Serial.println("---- Connected BNO Data ----");
  //bnoHandler.printConnectedBNOsData(0, 5);
  //Serial.println();

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

  logic.calculateTorque(data);

  delay(10);
}