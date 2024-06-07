#ifndef WIFI_SIMULATION_H
#define WIFI_SIMULATION_H

#include <Wire.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <string>
#include "enumIMU.h"
#include "define.h"

class WifiSimulation
{
private:
    // Json Variable to Hold Sensor Readings
    JSONVar readings;
    float RightHipAngle = 0;
    float RightKneeAngle = 0;
    float LeftHipAngle = 0;
    float LeftKneeAngle = 0;
    float SpineAngle = 0;


    const char *ssid = "BioGenius";
    const char *password = "biogenius!";
    long last_sent_millis = 0;


public:

    float toDegrees(float radians);
    String writeJson(); 
    void initWiFi();
    static void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void initSPIFFS();
    void wifiSetup();
    void setAngle(enumIMU imuType, float val);
    
};

#endif