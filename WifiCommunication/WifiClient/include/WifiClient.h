#ifndef WifiClient_h
#define WifiClient_h
#include <WiFi.h>
#include <WiFiUdp.h>
#include "enums.h"
#include <ArduinoJson.h>


class WifiClient 
{
    private:
        const char* ssid = "helloIAmUnder";  // WiFi network name
        const char* password = "ItsTricky"; // WiFi network password
        WiFiUDP UDP;
        unsigned int localUdpPort = 4211;  // Local port to receive responses
        void handShake();
        int IPsListSize = 0; // Number of IPs in the IPs_list
        IPAddress IPsList[10]; // 0 - watch, 1 - exoskeleton, 2 - simulation, 3+ - other clients
        void addIPAddress(IPAddress ip, enum IPType ID);
        IPAddress getIP(enum IPType index);
    public:
        WifiClient();
        void sendMessage(unsigned char data[], enum IPType address);
        void receiveMessage(unsigned char data[]);	
        bool isConnected();
        void wifiConnect(); 
        void wifiDisconnect();
        void wifiOff();
        void wifiOn();
};

#endif
