#include <WiFi.h>
#include <WiFiUdp.h>


class WifiClient 
{
    public:
        WifiClient();
        void sendMessage(unsigned char data[]);
        void receiveMessage(unsigned char data[]);
        bool isConnected();
    private:
        const char* ssid;  // WiFi network name
        const char* password;  // WiFi network password
        void connect();
        void disconnect();
        IPAddress serverIP; // IP address of the UDP server
        WiFiUDP UDP;
        unsigned int localUdpPort = 4211;  // Local port to receive responses (if needed)
        
};
