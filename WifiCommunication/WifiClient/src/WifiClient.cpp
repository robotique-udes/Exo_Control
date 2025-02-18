#include "Wificlient.h"
#include "WifiServer.h"
#include <stdexcept>
#define UDP_PORT_SEND 4210


WifiClient::WifiClient() // Constructor
{

}


void WifiClient::wifiConnect() // Connect to Wi-Fi
{

    // WiFi mode
    WiFi.mode(WIFI_STA);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.printf("Attempting to connect to \"%s\" with password \"%s\".\n\n", ssid, password);
    while (!isConnected()) 
    {
        delay(750);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi!");

    // Set up UDP
    UDP.begin(localUdpPort);
    Serial.println("UDP client started");

    // Handshake with server
    handShake();
}

void WifiClient::wifiDisconnect() // Disconnect from Wi-Fi
{
  WiFi.disconnect();
  Serial.println("Disconnected from WiFi.");
}


void WifiClient::sendMessage(int data_lenght, unsigned char data[], EnumIPType address) // Send message to server
{
    IPAddress sendingAddress;
    sendingAddress.fromString(IPsList[(int)EnumIPType::WATCH].c_str());
    UDP.beginPacket(sendingAddress, UDP_PORT_SEND);
    UDP.write(data, data_lenght);
    UDP.endPacket();
    Serial.printf("UDP sent packet contents: %s\n", data);
}

int WifiClient::dataAvailable() // Check if data is available
{
    return lenght_message_recieved = UDP.parsePacket();
}

void WifiClient::receiveMessage(unsigned char data[]) // Receive message from server
{
  unsigned char incomingPacket[lenght_message_recieved + 1];
  if (lenght_message_recieved) 
  {
    int len = UDP.read(incomingPacket, lenght_message_recieved);
    if (len > 0) {
        incomingPacket[len] = 0;
    }
    Serial.printf("UDP recieved packet contents: %s\n", incomingPacket);

    for(int i = 0; i < lenght_message_recieved; i++)
    {
        data[i] = incomingPacket[i];
    }
  }
}

bool WifiClient::isConnected() // Check if connected to Wi-Fi
{
    return WiFi.status() == WL_CONNECTED;
}

void WifiClient::wifiOff() // Turn off Wi-Fi to save power
{
    WifiClient::wifiDisconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi turned off.");
}

void WifiClient::wifiOn() // Turn on Wi-Fi
{
    WiFi.mode(WIFI_STA);
    WifiClient::wifiConnect();
    Serial.println("\nConnected to WiFi!");
}

void WifiClient::handShake() // Handshake with server
{   

    // Receive IP addresses
    int longueur_ips = 0;
    while (!longueur_ips) 
    {
        delay(800);
        longueur_ips = dataAvailable();
        Serial.print("Waiting for IP addresses... lentgh recieved : ");
        Serial.println(longueur_ips); 
    }
    unsigned char IPs[longueur_ips];
    receiveMessage(IPs);
    deserializeMessage(IPs, longueur_ips);

    // Set IP addresses (CAN BE CHANGED)
    std::pair<std::string, int> key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(EnumIPType::WATCH));
    std::string watch_ip = dataMap[key];
    IPsList[(int)EnumIPType::WATCH] = watch_ip;

    // convert string to const char*
    const char* watch_ip_char = watch_ip.c_str();

    // print IP address input
    Serial.print("Watch IP dataMap: ");
    Serial.print(watch_ip_char);
    Serial.println(".");

    // convert const char* to IPAddress
    IPAddress watch_ip_address;
    watch_ip_address.fromString(watch_ip_char);

    // Send connection confirmation
    unsigned char confirmation[22] = "Connection confirmed";
    sendMessage(22, confirmation, EnumIPType::WATCH);

}

void WifiClient::addIPAddress(IPAddress ip, EnumIPType ID) // Add IP address to list
{
    int index = (int)ID;
    IPsList[index] = ip;
    IPsListSize++;
}

std::string WifiClient::getIP(EnumIPType address) // Get IP address from list
{
    return IPsList[(int)address];
}

void WifiClient::deserializeMessage(unsigned char message[], int length)
{
    Serial.println("Data Map update");

    // deserialize message into a map
    JsonDocument doc;
    deserializeJson(doc, message);
    //serializeJson(doc, Serial);

    // extract data from message
    std::pair<std::string, int> key;
    std::string value;

    // logs
    JsonArray logs = doc[NESTED_LOGS];
    for (int i = 0; i < logs.size(); i++)
    {
        key = std::make_pair("logs", static_cast<int>(logs[i]["ID"]));
        value = logs[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        dataMap[key] = value;
    }

    // bnoAngles
    JsonArray bnoAngles = doc[NESTED_BNO_ANGLES];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(bnoAngles[i]["ID"]));
        value = bnoAngles[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        dataMap[key] = value;
    }

    // bnoPositions
    JsonArray bnoPositions = doc[NESTED_BNO_POSITIONS];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(bnoPositions[i]["ID"]));
        value = bnoPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

    // motorPositions
    JsonArray motorPositions = doc[NESTED_BNO_MOTEUR];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(motorPositions[i]["ID"]));
        value = motorPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

    // IP addresses
    JsonArray IPs = doc[NESTED_IP_TYPE];
    for (int i = 0; i < IPs.size(); i++)
    {
        key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(IPs[i]["ID"]));
        value = IPs[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

    Serial.println("Contents of the map:");
    for (const auto& entry : dataMap) {
        Serial.print("Key: (");
        Serial.print(entry.first.first.c_str());
        Serial.print(", ");
        Serial.print(entry.first.second);
        Serial.print(") -> Value: ");
        Serial.println(entry.second.c_str());
    }
    Serial.println(".");
}


