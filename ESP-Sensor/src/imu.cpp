#include "imu.h"


// Create AsyncWebServer object on port 80
// WebServer server(80);
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

//double height;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient *globalClient = NULL;

// Create an Event Source on /events
AsyncEventSource events("/events");

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
// TwoWire BUS_I2C_1 = TwoWire(0);
// TwoWire BUS_I2C_2 = TwoWire(1);
// TwoWire BUS_I2C_3 = TwoWire(2);

// Adafruit_BNO055 IMU_HAUT_D = Adafruit_BNO055(55, 0x29, &BUS_I2C_2);
// Adafruit_BNO055 IMU_BAS_D = Adafruit_BNO055(55, 0x28, &BUS_I2C_2);
// Adafruit_BNO055 IMU_DOS = Adafruit_BNO055(55, 0x28, &BUS_I2C_3);

Imu::Imu()
{
    
}

Imu::~Imu()
{
}

void Imu::wifiSetup()
{
    initWiFi();
    initSPIFFS();
    ws.onEvent(&Imu::onWsEvent);
    server.addHandler(&ws);
    server.begin();
}

bool Imu::IMUSetup()
{
    multiplex.selectChannel(0);
    IMU_HAUT_G = Adafruit_BNO055(55, 0x28, &Wire);
    multiplex.selectChannel(1);
    IMU_BAS_G = Adafruit_BNO055(55, 0x29, &Wire);

    Serial.println("Orientation Sensor Test");
    Serial.println("");
    Wire.begin(I2C_SDA1, I2C_SCL1);
    // BUS_I2C_2.begin(I2C_SDA2, I2C_SCL2);
    // BUS_I2C_3.begin(I2C_SDA3, I2C_SCL3);
    Serial.println("-------1");
    /* Initialise the sensor */
    multiplex.selectChannel(0);
    if (!IMU_HAUT_G.begin())
    {
        //There was a problem detecting the BNO055 ... check your connections 
        Serial.println("Ooops, no BNO055_1 detected ... Check your wiring or I2C ADDR!");
        
        // return false;
    }
    Serial.println("-------2");
    multiplex.selectChannel(1);
    if (!IMU_BAS_G.begin())
    {
        //There was a problem detecting the BNO055 ... check your connections 
        Serial.println("Ooops, no BNO055_2 detected ... Check your wiring or I2C ADDR!");
        // return false;
    }
    // Serial.println("-------3");
    // if (!IMU_HAUT_D.begin())
    // {
    //     //There was a problem detecting the BNO055 ... check your connections 
    //     Serial.println("Ooops, no BNO055_3 detected ... Check your wiring or I2C ADDR!");
    //     // return false;
    // }
    // Serial.println("-------4");
    // if (!IMU_BAS_D.begin())
    // {
    //     // There was a problem detecting the BNO055 ... check your connections 
    //     Serial.println("Ooops, no BNO055_4 detected ... Check your wiring or I2C ADDR!");
    //     // return false;
    // }
    Serial.println("-------5");
    /*if (!IMU_DOS.begin())
    {
        //There was a problem detecting the BNO055 ... check your connections 
        Serial.println("Ooops, no BNO055_5 detected ... Check your wiring or I2C ADDR!");
        // return false;
    }*/

    Serial.println("Setup done");
    /* Use external crystal for better accuracy */
    IMU_HAUT_G.setExtCrystalUse(true);
    // IMU_BAS_G.setExtCrystalUse(true);
    //IMU_HAUT_D.setExtCrystalUse(true);
    //IMU_BAS_D.setExtCrystalUse(true);
    //IMU_DOS.setExtCrystalUse(true);
    return true;
}

float Imu::toDegrees(float radians)
{
    return radians * 180 / PI;
}

// get angles from sensor
void Imu::getAngles()
{

    angleHipL = ImuAngle(&IMU_HAUT_G, 0);
    angleHipL.y()=angleHipL.y()+PI/2;

    angleKneeL = ImuAngle(&IMU_BAS_G,1);
    angleKneeL.y()=abs((angleKneeL.y()-angleHipL.y())+PI/2);


    //quat = IMU_HAUT_D.getQuat();
    //angleHipR = quat.toEuler();
    //angleHipR.y()=angleHipR.y()+PI/2;

    //quat = IMU_BAS_D.getQuat();
    //angleKneeR = quat.toEuler();
    //angleKneeR.y()=abs((angleKneeR.y()-angleHipR.y())+PI/2);

    //quat = IMU_DOS.getQuat();
    //SPLINE = quat.toEuler();
}

void Imu::printAngles()
{
    getAngles();
    Serial.print("angleHipL Y: ");
    Serial.print(toDegrees(angleHipL.y()));

    Serial.print("  angleKneeL Y: ");
    Serial.print(toDegrees(angleKneeL.y()));

    //Serial.print("  angleHipR Y: ");
    //Serial.print(toDegrees((angleHipR.y())));

    //Serial.print("  angleKneeR Y: ");
    //Serial.print(toDegrees(angleKneeR.y()));

    //Serial.print("SPLINE: ");
    //Serial.println(SPLINE.x());
}

String Imu::writeJson()
{
    getAngles();
    readings["G_ALPHA_X"] = toDegrees(angleHipL.x());
    readings["G_ALPHA_Y"] = toDegrees(angleHipL.y());
    readings["G_ALPHA_Z"] = toDegrees(angleHipL.z());

    readings["G_BETA_X"] = toDegrees(angleKneeL.x());
    readings["G_BETA_Y"] = toDegrees(angleKneeL.y());
    readings["G_BETA_Z"] = toDegrees(angleKneeL.z());

    readings["D_ALPHA_X"] = toDegrees(angleHipR.x());
    readings["D_ALPHA_Y"] = toDegrees(angleHipR.y());
    readings["D_ALPHA_Z"] = toDegrees(angleHipR.z());

    readings["D_BETA_X"] = toDegrees(angleKneeR.x());
    readings["D_BETA_Y"] = toDegrees(angleKneeR.y());
    readings["D_BETA_Z"] = toDegrees(angleKneeR.z());

    //readings["SPINE_X"] = toDegrees(SPLINE.x());
    //readings["SPINE_Y"] = toDegrees(SPLINE.y());
    //readings["SPINE_Z"] = toDegrees(SPLINE.z());

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

// Initialize WiFi
void Imu::initWiFi()
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

void Imu::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    Imu imu;

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

void Imu::initSPIFFS()
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

double Imu::getValAngle(enumIMU imuType)
{
    switch (imuType)
    {
    case enumIMU::HipL:
        return (angleHipL.y());
        break;
    case enumIMU::KneeL:
        return (angleKneeL.y());
        break;
    case enumIMU::HipR:
        return (angleHipR.y());
        break;
    case enumIMU::KneeR:
        return (angleKneeR.y());
        break;
    default:
        return 0;
        break;
    }
}
imu::Vector<3> Imu::ImuAngle(Adafruit_BNO055 *imuCourant, uint8_t chan)
{
    multiplex.selectChannel(chan);
    quat = imuCourant->getQuat();
    multiplex.selectChannel(0);
    return quat.toEuler();

}

