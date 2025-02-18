#ifndef WIFI_SERVER
#define WIFI_SERVER

#include <WiFi.h>
#include <WiFiUdp.h>
#include <map>
#include <string>
#include "enums.h"
#include "MessageBuilder/MessageBuilder.h"

#define SSID "helloIAmUnder"
#define PASS_PHRASE "ItsTricky"

#define UDP_PORT_RECEIVE 4210
#define IP_LIST_SIZE 10
#define INTERVAL_1SEC 1000
#define INTERVAL_10ms 10

#define ENUM_BNO_ANGLE "EnumBnoAngle"
#define ENUM_BNO_POSITION "EnumBnoPosition"
#define ENUM_MOTOR_POSITION "EnumMotorPosition"
#define ENUM_IP_TYPE "EnumIpType"

struct IpTypeList
{
    EnumIPType ipType;
    IPAddress ipAdresse;
    ip4_addr_t ipAdd;
    uint8_t mac[6];
};

class WifiServer
{
private:
    WiFiUDP UDP;
    char packet[255];
    IPAddress Local_ip;
    IPAddress Gateway;
    IPAddress Subnet;
    IPAddress MyIP;
    std::map<std::pair<std::string, int>, std::string> unifiedMap;

    const char* ServerSSID;
    const char* ServerPassword;

    void handShake();
    void onWiFiEvent(WiFiEvent_t event);
    void InitialiseIPList();
    void newClientConnection(IpTypeList newClient);
    

    // Private constructors to prevent instantiation from outside
    WifiServer();
    WifiServer(char* ssid, char* passphrase);

    // Deleted copy constructor and assignment operator for singleton pattern
    WifiServer(const WifiServer&) = delete;
    WifiServer& operator=(const WifiServer&) = delete;

    static WifiServer* wifiInstance;

public:
    IpTypeList IPsList[IP_LIST_SIZE];
    unsigned char lastMessage[800];
    int lastMessageLength;
    uint8_t numClient;
    unsigned char readyToSendHandShake;
    unsigned char timerBeforeSendinghandShake;
    // Static method to access the singleton instance
    static WifiServer* GetInstance();

    ~WifiServer();
    int Initialise();
    int DataAvailable();
    int ReadData(int);
    int SendData(unsigned char* packet, int length);
    IPAddress getIP(EnumIPType index);

    int retrieveInformation(EnumBnoAngle BNO_NAME, float* value);
    int retrieveInformation(EnumBnoPosition BNO_NAME, float* value);
    int retrieveInformation(EnumMotorPosition MOTOR_NAME, float* value);
    int retrieveInformation(EnumIPType IP_NAME, IPAddress* value);

    void deserializeMessage(unsigned char message[], int length);

    static void upDate();
};

#endif
