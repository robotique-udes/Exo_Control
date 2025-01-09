#ifndef WIFI_SERVER
#define WIFI_SERVER

#include <WiFi.h>
#include <WiFiUdp.h>
#include "enums.h"

#define UDP_PORT_RECEIVE 4210
#define IP_LIST_SIZE 10

struct IpTypeList
{
    EnumIPType ipType;
    IPAddress ipAdresse;
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
    
    uint8_t numClient;

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
    unsigned char lastMessage[255];
    int lastMessageLength;

    // Static method to access the singleton instance
    static WifiServer* GetInstance(char* ssid, char* passphrase);

    ~WifiServer();
    int Initialise();
    int DataAvailable();
    int ReadData(int);
    int SendData(unsigned char* packet, int length);
    IPAddress getIP(EnumIPType index);
};

#endif
