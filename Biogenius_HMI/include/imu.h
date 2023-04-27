#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <string>

#define I2C_SDA1 27
#define I2C_SCL1 26
#define I2C_SDA2 33
#define I2C_SCL2 32
#define I2C_SDA3 25
#define I2C_SCL3 14

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (800)

const char *ssid = "BioGenius";
const char *password = "biogenius!";
long last_sent_millis = 0;
String getAngles();

// Create AsyncWebServer object on port 80
// WebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient *globalClient = NULL;

// Create an Event Source on /events
AsyncEventSource events("/events");

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
TwoWire BUS_I2C_1 = TwoWire(0);
TwoWire BUS_I2C_2 = TwoWire(1);
TwoWire BUS_I2C_3 = TwoWire(2);

Adafruit_BNO055 IMU_HAUT_G = Adafruit_BNO055(55, 0x28, &BUS_I2C_1);
Adafruit_BNO055 IMU_BAS_G = Adafruit_BNO055(55, 0x29, &BUS_I2C_1);
Adafruit_BNO055 IMU_HAUT_D = Adafruit_BNO055(55, 0x28, &BUS_I2C_1);
Adafruit_BNO055 IMU_BAS_D = Adafruit_BNO055(55, 0x28, &BUS_I2C_2);
Adafruit_BNO055 IMU_DOS = Adafruit_BNO055(55, 0x28, &BUS_I2C_3);


class IMU
{
private:
    // Json Variable to Hold Sensor Readings
    JSONVar readings;
    IMU();
    ~IMU();

public:
    float toDegrees(float radians);
    String getAngles();
    void initWiFi();
    static void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void initSPIFFS();
    void IMUSetup();
};

#endif