#include <Arduino.h>
#include <Wire.h>
#include "bnoHandler.h"

static BnoHandler bnoHandler;

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

  Serial.println("---- Connected BNO Data ----");
  bnoHandler.printConnectedBNOsData(0, 4);
  bnoHandler.printGroundState();
  Serial.println();

  delay(200);
}