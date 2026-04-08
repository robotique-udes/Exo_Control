#include <Wire.h>
#include <Adafruit_BNO08x.h>

// TCA9546A and BNO085 addresses
#define TCA_ADDR 0x70      // TCA9546A default address
#define BNO_ADDR 0x4A      // BNO085 default address
#define NUM_BNO 4          // Number of BNO085 sensors (channels 0-3)

Adafruit_BNO08x bno08x[NUM_BNO];
sh2_SensorValue_t sensorValues[NUM_BNO];

// Function to select I2C channel (0-3 for TCA9546A)
void selectChannel(uint8_t i) {
  if (i > 3) return;

  // First, disable all channels
 // Wire.beginTransmission(TCA_ADDR);
 // Wire.write(0); 
 // Wire.endTransmission();

  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(3000);

  // Initialize all BNO085 sensors on channels 0-3
  for (uint8_t ch = 0; ch < NUM_BNO; ch++) {
    if (ch == 2)
      continue;
    selectChannel(ch);
    delay(100); // Small delay for channel switching
    if (!bno08x[ch].begin_I2C(BNO_ADDR)) {
      Serial.print("Failed to find BNO085 chip on Channel ");
      Serial.println(ch);
    } else {
      Serial.print("ok to find BNO085 chip on Channel ");
      Serial.println(ch);
      bno08x[ch].enableReport(SH2_ROTATION_VECTOR);
    }
    delay(10);
  }
}

void loop() {
  // Example: Read all BNO085 sensors in sequence
  for (uint8_t ch = 0; ch < NUM_BNO; ch++) {
    if (ch == 2)
      continue;

    selectChannel(ch);
    delay(20); // Allow channel switch to settle
    if (bno08x[ch].getSensorEvent(&sensorValues[ch])) {
      if (sensorValues[ch].sensorId == SH2_ROTATION_VECTOR) {
        Serial.print("Channel ");
        Serial.print(ch);
        Serial.print(" Quat: ");
        Serial.print(sensorValues[ch].un.rotationVector.i);
        Serial.print(", ");
        Serial.print(sensorValues[ch].un.rotationVector.j);
        Serial.print(", ");
        Serial.println(sensorValues[ch].un.rotationVector.k);
      }
    }
    delay(10); // Optional: adjust for sensor timing
  }
  delay(100); // Main loop delay
}