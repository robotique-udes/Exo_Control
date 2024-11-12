#ifndef WifiClient_h
#define WifiClient_h
#include <WiFi.h>
#include <WiFiUdp.h>


class WifiClient 
{
    public:
        WifiClient();
        void sendMessage(unsigned char data[]);
        void receiveMessage(unsigned char data[]);
        bool isConnected();
        void wifiConnect(); 
        void wifiDisconnect();
        void wifiOff();
        void wifiOn();
    private:
        const char* ssid = "helloIAmUnder";  // WiFi network name
        const char* password = "ItsTricky"; // WiFi network password
        IPAddress serverIP; // IP address of the UDP server
        WiFiUDP UDP;
        unsigned int localUdpPort = 4211;  // Local port to receive responses (if needed)
};

#endif
