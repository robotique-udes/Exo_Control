#include "imu.h"

IMU::IMU()
{
}

IMU::~IMU()
{
}

void IMU::IMUSetup()
{
    initWiFi();
    initSPIFFS();
    ws.onEvent(&IMU::onWsEvent);
    server.addHandler(&ws);
    server.begin();

    Serial.println("Orientation Sensor Test");
    Serial.println("");

    BUS_I2C_1.begin(I2C_SDA1, I2C_SCL1);
    BUS_I2C_2.begin(I2C_SDA2, I2C_SCL2);
    BUS_I2C_3.begin(I2C_SDA3, I2C_SCL3);

    /* Initialise the sensor */
    if (!IMU_HAUT_G.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055_1 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    if (!IMU_BAS_G.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055_2 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    if (!IMU_HAUT_D.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055_3 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    if (!IMU_BAS_D.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055_4 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    if (!IMU_DOS.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055_5 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    /* Use external crystal for better accuracy */
    IMU_HAUT_G.setExtCrystalUse(true);
    IMU_BAS_G.setExtCrystalUse(true);
    IMU_HAUT_D.setExtCrystalUse(true);
    IMU_BAS_D.setExtCrystalUse(true);
    IMU_DOS.setExtCrystalUse(true);
}

float IMU::toDegrees(float radians)
{
    return radians * 180 / PI;
}

// get angles from sensor
String IMU::getAngles()
{
    sensors_event_t event;
    imu::Quaternion quat;
    imu::Quaternion quat2;
    imu::Vector<3> g_alpha;
    imu::Vector<3> g_beta;
    imu::Vector<3> d_alpha;
    imu::Vector<3> d_beta;
    imu::Vector<3> SPLINE;

    quat = IMU_HAUT_G.getQuat();
    g_alpha = quat.toEuler();
    readings["G_ALPHA_X"] = toDegrees(g_alpha.x());
    readings["G_ALPHA_Y"] = toDegrees(g_alpha.y());
    readings["G_ALPHA_Z"] = toDegrees(g_alpha.z());

    quat = IMU_BAS_G.getQuat();
    g_beta = quat.toEuler();
    readings["G_BETA_X"] = toDegrees(g_beta.x());
    readings["G_BETA_Y"] = toDegrees(g_beta.y());
    readings["G_BETA_Z"] = toDegrees(g_beta.z());

    quat = IMU_HAUT_D.getQuat();
    d_alpha = quat.toEuler();
    readings["D_ALPHA_X"] = toDegrees(d_alpha.x());
    readings["D_ALPHA_Y"] = toDegrees(d_alpha.y());
    readings["D_ALPHA_Z"] = toDegrees(d_alpha.z());

    quat = IMU_BAS_D.getQuat();
    d_beta = quat.toEuler();
    readings["D_BETA_X"] = toDegrees(d_beta.x());
    readings["D_BETA_Y"] = toDegrees(d_beta.y());
    readings["D_BETA_Z"] = toDegrees(d_beta.z());

    quat = IMU_DOS.getQuat();
    SPLINE = quat.toEuler();
    readings["SPINE_X"] = toDegrees(SPLINE.x());
    readings["SPINE_Y"] = toDegrees(SPLINE.y());
    readings["SPINE_Z"] = toDegrees(SPLINE.z());

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

// Initialize WiFi
void IMU::initWiFi()
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
        // delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Hello World, I'm connected to the internets!!");
}

void IMU::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    IMU imu;

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
            String angles = imu.getAngles();
            Serial.println(angles);
            globalClient->text(angles);
        }
        else
        {
            Serial.println("An error occured when sending to client");
        }
    }
}

void initSPIFFS()
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