#ifndef WifiClient_h
#define WifiClient_h
#include <WiFi.h>
#include <WiFiUdp.h>
#include <map>
#include "enums.h"
#include "../libs/MessageBuilder/MessageBuilder.h"
             
#define ENUM_BNO_ANGLE "EnumBnoAngle"
#define ENUM_BNO_POSITION "EnumBnoPosition"
#define ENUM_MOTOR_POSITION "EnumMotorPosition"
#define ENUM_IP_TYPE "EnumIpType"
#define ENUM_INFO_TYPE "EnumInfoType"

class WifiClient 
{
private:
    
    // Deleted copy constructor and assignment operator for singleton pattern
    static WifiClient* clientInstance;
    
    WiFiUDP UDP;
    const char* ssid = "helloIAmUnder";  // WiFi network name
    const char* password = "ItsTricky"; // WiFi network password
    int lenght_message_recieved = 0;
    unsigned char message_recieved[1024];
    unsigned int localUdpPort = 4211;  // Local port to receive responses
    bool handShakeDone = false;
    bool tryingToConnect = false;
    int IPsListSize = 0; // Number of IPs in the IPs_list
    //std::string IPsList[10]; // 0 - watch, 1 - exoskeleton, 2 - simulation, 3+ - other clients
    std::map<std::pair<std::string, int>, std::string> dataMap;
    
    // Private constructors to prevent instantiation from outside
    WifiClient();
    void addIPAddress(IPAddress ip, EnumIPType ID);
    void deserializeMessage(unsigned char message[], int length);
    void handShake();
    int dataAvailable();
    std::string getIP(EnumIPType index);
    WifiClient(const WifiClient&) = delete;
    WifiClient& operator=(const WifiClient&) = delete;
    
public:
    // Static method to access the singleton instance
    static WifiClient* GetInstance();
    ~WifiClient();
    
    std::string IPsList[10]; // 0 - watch, 1 - exoskeleton, 2 - simulation, 3+ - other clients

    void sendMessage(int data_lenght, unsigned char data[], EnumIPType address);
    void receiveMessage();	
    bool isConnected();
    void wifiConnect(); 
    void wifiDisconnect();
    void wifiOff();
    void wifiOn();
    static void upDate();
};

#endif
