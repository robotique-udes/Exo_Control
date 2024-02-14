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
#include "enum.h"
#include "bnoMultiplex.h"
#include "define.h"

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

    //BNOMultiplex IMU_HAUT_G =  BNOMultiplex(55, 0x28, &Wire, 0);
    //BNOMultiplex IMU_BAS_G = BNOMultiplex(55, 0x29, &Wire, 1);

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
    double getValAngle(enumIMU imuType);
    void printSonar();

    
};

#endif