#include <Arduino.h>
/**
 * TCA9548 I2CScanner.ino -- I2C bus scanner for Arduino
 *
 * Based on https://playground.arduino.cc/Main/I2cScanner/
 *
 */

#include "Wire.h"

#define PCAADDR 0x70
#define SDA_2 21
#define SCL_2 22

void pcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


// standard Arduino setup()
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("HELLO_WORLD!");
  Wire.setPins(SDA_2, SCL_2);
  Wire.begin();


  Serial.println("Scanning for Onboard I2C devices...");
}

void tryStuff() {

  Wire.beginTransmission(0x20);
  if (!Wire.endTransmission()) {
    Serial.println("Extra GPIO Found I2C @ 0x20");
  }

  Wire.beginTransmission(0x7C);
  if (!Wire.endTransmission()) {
    Serial.println("PWM Found I2C @ 0x7C");
  }

  Wire.beginTransmission(0x70);
  if (!Wire.endTransmission()) {
    Serial.println("Found I2C Multiplexer @ 0x70");
  }

  Wire.beginTransmission(0x0);
  if (!Wire.endTransmission()) {
    Serial.println("Found Generic Address @ 0x0");
  }

  Wire.beginTransmission(0x48);
  if (!Wire.endTransmission()) {
    Serial.println("Found Ghost Address @ 0x48");
  }
  
  for (uint8_t t=0; t<8; t++) {
    pcaselect(t);
    Serial.print("PCA Port #"); Serial.println(t);

    for (uint8_t addr = 0; addr<=127; addr++) {
      if (addr == PCAADDR) continue;
      if (addr == 0x20) continue;
      if (addr == 0x7C) continue;
      if (addr == 0x0) continue;
      if (addr == 0x48) continue;

      Wire.beginTransmission(addr);
      if (!Wire.endTransmission()) {
        Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
      }
    }
  }

}

void loop() 
{
  tryStuff();
  delay(5000);
  Serial.println("");
}