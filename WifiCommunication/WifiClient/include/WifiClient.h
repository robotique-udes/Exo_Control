#ifndef WifiClient_h
#define WifiClient_h
#include <WiFi.h>
#include <WiFiUdp.h>
#include <map>
#include "enums.h"
#include "MessageBuilder/MessageBuilder.h"
             
#define ENUM_BNO_ANGLE "EnumBnoAngle"
#define ENUM_BNO_POSITION "EnumBnoPosition"
#define ENUM_MOTOR_POSITION "EnumMotorPosition"
#define ENUM_IP_TYPE "EnumIpType"

class WifiClient 
{
    private:
        const char* ssid = "helloIAmUnder";  // WiFi network name
        const char* password = "ItsTricky"; // WiFi network password
        int lenght_message_recieved = 0;
        WiFiUDP UDP;
        unsigned int localUdpPort = 4211;  // Local port to receive responses
        void handShake();
        int IPsListSize = 0; // Number of IPs in the IPs_list
        IPAddress IPsList[10]; // 0 - watch, 1 - exoskeleton, 2 - simulation, 3+ - other clients
        void addIPAddress(IPAddress ip, EnumIPType ID);
        IPAddress getIP(EnumIPType index);
        std::map<std::pair<std::string, int>, std::string> dataMap;
        void deserializeMessage(unsigned char message[], int length);
        int dataAvailable();
    public:
        WifiClient();
        void sendMessage(int data_lenght, unsigned char data[], EnumIPType address);
        void receiveMessage(unsigned char data[]);	
        bool isConnected();
        void wifiConnect(); 
        void wifiDisconnect();
        void wifiOff();
        void wifiOn();
};

#endif
