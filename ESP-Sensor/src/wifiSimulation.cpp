#include "WifiSimulation.h"

// Create AsyncWebServer object on port 80
// WebServer server(80);
//IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
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

float WIFI_THIGH_LX = 0;
float WIFI_TIBIA_LX = 0;
float WIFI_THIGH_RX = 0;
float WIFI_TIBIA_RX = 0;
float WIFI_BACKX = 0;

float WIFI_THIGH_LZ = 0;
float WIFI_TIBIA_LZ = 0;
float WIFI_THIGH_RZ = 0;
float WIFI_TIBIA_RZ = 0;
float WIFI_BACKZ = 0;

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

String WifiSimulation::writeJson() // convertie les valeurs des variables suivantes en une string sous le format d'un JSON
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

// Les variables WIFI sont assigné a des clés JSON
    readings["TIBIA_L"] = WIFI_TIBIA_L;
    readings["TIBIA_R"] = WIFI_TIBIA_R;
    readings["THIGH_L"] = WIFI_THIGH_L;
    readings["THIGH_R"] = WIFI_THIGH_R;
    readings["BACK"] = WIFI_BACK;

    //--------WIFI X-------//
    readings["TIBIA_LX"] = WIFI_TIBIA_LX;
    readings["TIBIA_RX"] = WIFI_TIBIA_RX;
    readings["THIGH_LX"] = WIFI_THIGH_LX;
    readings["THIGH_RX"] = WIFI_THIGH_RX;
    readings["BACKX"] = WIFI_BACKX;

    //--------WIFI Y-------//
    readings["TIBIA_LZ"] = WIFI_TIBIA_LZ;
    readings["TIBIA_RZ"] = WIFI_TIBIA_RZ;
    readings["THIGH_LZ"] = WIFI_THIGH_LZ;
    readings["THIGH_RZ"] = WIFI_THIGH_RZ;
    readings["BACKZ"] = WIFI_BACKZ;

//Convertion de l'objet reading en une string format JSON
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

void WifiSimulation::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)//cette fonction ce fait appelé tout seul asynchrone
{
    WifiSimulation imu;//Weird que il y ait un objet WifiSimulation dans la classe WifiSimulation

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
    else if (type == WS_EVT_DATA)//SI rentre, cela signifie que des données ont été reçues du client WebSocket.
    {
        if (globalClient != NULL && globalClient->status() == WS_CONNECTED)
        {
            String angles = imu.writeJson();//pourquoi pas juste un this->writeJson
            // Serial.println(angles);
            globalClient->text(angles);// fait simplement envoyé la string (le JSON) au client via le WebSocket.
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
    case enumIMU::THIGH_LX:
        WIFI_THIGH_LX = val;
        break;
    case enumIMU::TIBIA_LX:
        WIFI_TIBIA_LX = val;
        break;
    case enumIMU::THIGH_RX:
        WIFI_THIGH_RX = val;
        break;
    case enumIMU::TIBIA_RX:
        WIFI_TIBIA_RX = val;  
        break;
    case enumIMU::EXO_BACKX:
        WIFI_BACKX = val;  
        break;
    case enumIMU::THIGH_LZ:
        WIFI_THIGH_LZ = val;
        break;
    case enumIMU::TIBIA_LZ:
        WIFI_TIBIA_LZ = val;
        break;
    case enumIMU::THIGH_RZ:
        WIFI_THIGH_RZ = val;
        break;
    case enumIMU::TIBIA_RZ:
        WIFI_TIBIA_RZ = val;  
        break;
    case enumIMU::EXO_BACKZ:
        WIFI_BACKZ = val;    
        break;
    default:
        break;
    }
}