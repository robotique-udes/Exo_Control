#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <Wire.h>


// This is where the multiplexer is connected
#define SDA_2 26
#define SCL_2 27
#define PCAADDR 0x70

// IMU data
#define BNO08X_RESET -1

#define BNO_ONE_MULTIPLEXER_PORT 0
#define BNO_ONE_I2C_ADDRESS 0x4B
#define BNO_TWO_MULTIPLEXER_PORT 1
#define BNO_Two_I2C_ADDRESS 0x4A

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

// Top frequency is about 250Hz but this report is more accurate
sh2_SensorId_t reportType = SH2_GAME_ROTATION_VECTOR;
long reportIntervalUs = 5000; // Frequency can be set here, ideally we never go past 200Hz

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(reportType, reportIntervalUs)) {
    Serial.println("Could not enable reporting with the given type. Check your desired report type and interval.");
  }
}

// This will select the given port on the multiplexer
void PCA_Select(uint8_t port) {
  if (port > 7) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << port);
  Wire.endTransmission();  
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test through PCA9548A Multiplexer!");

  // Initialize the multiplexer
  Wire.begin(SDA_2, SCL_2);

  // Testing the connection to the multiplexer
  Wire.beginTransmission(PCAADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("PCA9548A Multiplexer found!");
  } else {
    Serial.println("PCA9548A Multiplexer not found!");
    while (1) { delay(100); };
  }

  // First test, only one IMU
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << BNO_ONE_MULTIPLEXER_PORT);
  Wire.endTransmission();

  // Try to initialize!
  if (!bno08x.begin_I2C(BNO_ONE_I2C_ADDRESS)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(100); }
  }
  Serial.println("BNO_ONE Found!");

  setReports();

  Serial.println("Reading events");
  delay(100);
}

void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }
  
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  
  switch (sensorValue.sensorId) {
    
    case SH2_GAME_ROTATION_VECTOR:
      Serial.print("Game Rotation Vector - r: ");
      Serial.print(sensorValue.un.gameRotationVector.real);
      Serial.print(" i: ");
      Serial.print(sensorValue.un.gameRotationVector.i);
      Serial.print(" j: ");
      Serial.print(sensorValue.un.gameRotationVector.j);
      Serial.print(" k: ");
      Serial.println(sensorValue.un.gameRotationVector.k);
      break;
  }

}