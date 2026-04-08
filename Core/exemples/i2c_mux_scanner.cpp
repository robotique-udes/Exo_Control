#include <Wire.h>
#include <Arduino.h>

#define MAIN_I2C_SDA 11
#define MAIN_I2C_SCL 12
#define MUX_ADDR 0x70 // Default TCA9546A address, change if needed
#define MUX_CHANNEL 3  // Channel to scan (0-3)

void selectMuxChannel(uint8_t channel) {
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("I2C Scanner with TCA9546A Mux");
  Wire.setPins(MAIN_I2C_SDA, MAIN_I2C_SCL);
  Wire.begin();
  selectMuxChannel(MUX_CHANNEL);
  Serial.print("Scanning on mux channel ");
  Serial.println(MUX_CHANNEL);
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000); // Wait 5 seconds before next scan
}
