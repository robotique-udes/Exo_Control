#ifndef IMU_H
#define IMU_H

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

#define I2C_SDA1 26
#define I2C_SCL1 27
#define I2C_SDA2 33
#define I2C_SCL2 32
#define I2C_SDA3 25
#define I2C_SCL3 14



/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (800)

class Imu
{
private:
    // Json Variable to Hold Sensor Readings
    JSONVar readings;
    sensors_event_t event;
    imu::Quaternion quat;
    imu::Quaternion quat2;
    imu::Vector<3> angleHipL;
    imu::Vector<3> angleKneeL;
    imu::Vector<3> angleHipR;
    imu::Vector<3> angleKneeR;
    imu::Vector<3> SPLINE;

    const char *ssid = "BioGenius";
    const char *password = "biogenius!";
    long last_sent_millis = 0;


public:
    Imu();
    ~Imu();
    float toDegrees(float radians);
    void getAngles();
    String writeJson();
    void initWiFi();
    void printAngles();
    static void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void initSPIFFS();
    bool IMUSetup();
    void wifiSetup();
};

#endif