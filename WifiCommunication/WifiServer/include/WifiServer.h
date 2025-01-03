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
};


class  WifiServer
{
private:
    WiFiUDP UDP;
    char packet[255];
    //char reply[] = "Packet received!";

    // WiFiServer server(80);
    // WiFiClient client;
    IPAddress Local_ip;
    IPAddress Gateway;
    IPAddress Subnet;
    IPAddress MyIP;
    
    IpTypeList IPsList[IP_LIST_SIZE];

    const char* ServerSSID;
    const char* ServerPassword;

    void handShake();
    void onWiFiEvent(WiFiEvent_t event);
    void InitialiseIPList();

public:
    unsigned char lastMessage[255];
    int lastMessageLength;


     WifiServer(const char* ssid, const char* passphrase);
    ~ WifiServer();
     int Initialise();
     int DataAvailable();
     int ReadData(int);
     int SendData(unsigned char * packet, int length);
    IPAddress getIP(EnumIPType index);
};





#endif