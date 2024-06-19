#include "WifiSimulation.h"

// Create AsyncWebServer object on port 80
// WebServer server(80);
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// double height;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient *globalClient = NULL;

// Create an Event Source on /events
AsyncEventSource events("/events");

// global angle variables
float WIFI_THIGH_L = 0;
float WIFI_TIBIA_L = 0;
float WIFI_THIGH_R = 0;
float WIFI_TIBIA_R = 0;
float WIFI_BACK = 0;

void WifiSimulation::wifiSetup()
{
    initWiFi();
    initSPIFFS();
    ws.onEvent(&WifiSimulation::onWsEvent);
    server.addHandler(&ws);
    server.begin();
}

float WifiSimulation::toDegrees(float radians)
{
    return radians * 180 / PI;
}

String WifiSimulation::writeJson()
{
    // readings["G_ALPHA_X"] = toDegrees(angleHipL.x());
    // readings["G_ALPHA_Y"] = LeftKneeAngle;
    // readings["G_ALPHA_Z"] = toDegrees(angleHipL.z());

    // readings["G_BETA_X"] = toDegrees(angleKneeL.x());
    // readings["G_BETA_Y"] = LeftHipAngle;
    // readings["G_BETA_Z"] = toDegrees(angleKneeL.z());

    // readings["D_ALPHA_X"] = toDegrees(angleHipR.x());
    // readings["D_ALPHA_Y"] = RightKneeAngle;
    // readings["D_ALPHA_Z"] = toDegrees(angleHipR.z());

    // readings["D_BETA_X"] = toDegrees(angleKneeR.x());
    // readings["D_BETA_Y"] = RightHipAngle;
    // readings["D_BETA_Z"] = toDegrees(angleKneeR.z());

    // readings["SPINE_X"] = toDegrees(SPLINE.x());
    // readings["SPINE_Y"] = toDegrees(SPLINE.y());
    // readings["SPINE_Z"] = toDegrees(SPLINE.z());

    readings["TIBIA_L"] = WIFI_TIBIA_L;
    readings["TIBIA_R"] = WIFI_TIBIA_R;
    readings["THIGH_L"] = WIFI_THIGH_L;
    readings["THIGH_R"] = WIFI_THIGH_R;
    readings["BACK"] = WIFI_BACK;

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

// Initialize WiFi
void WifiSimulation::initWiFi()
{

    // Connect to Wifi.
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    // delay(100);

    WiFi.begin(ssid, password);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED)
    {
        // Check to see if connecting failed.
        // This is due to incorrect credentials
        if (WiFi.status() == WL_CONNECT_FAILED)
        {
            Serial.println("Failed to connect to WIFI. Please verify credentials: ");
            Serial.println();
            Serial.print("SSID: ");
            Serial.println(ssid);
            Serial.print("Password: ");
            Serial.println(password);
            Serial.println();
        }
        delay(10);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Hello World, I'm connected to the internets!!");
}

void WifiSimulation::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    WifiSimulation imu;

    if (type == WS_EVT_CONNECT)
    {

        Serial.println("Websocket client connection received");
        globalClient = client;
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        globalClient = NULL;
        Serial.println("Client disconnected");
        Serial.println("-----------------------");
    }
    else if (type == WS_EVT_DATA)
    {
        if (globalClient != NULL && globalClient->status() == WS_CONNECTED)
        {
            String angles = imu.writeJson();
            // Serial.println(angles);
            globalClient->text(angles);
        }
        else
        {
            Serial.println("An error occured when sending to client");
        }
    }
}

void WifiSimulation::initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    else
    {
        Serial.println("SPIFFS mounted successfully");
    }
}

void WifiSimulation::setAngle(enumIMU imuType, float val)
{
    switch (imuType)
    {
    case enumIMU::THIGH_L:
        WIFI_THIGH_L = val;
        break;
    case enumIMU::TIBIA_L:
        WIFI_TIBIA_L = val;
        break;
    case enumIMU::THIGH_R:
        WIFI_THIGH_R = val;
        break;
    case enumIMU::TIBIA_R:
        WIFI_TIBIA_R = val;
        break;
    case enumIMU::EXO_BACK:
        WIFI_BACK = val;
        break;
    default:
        break;
    }
}