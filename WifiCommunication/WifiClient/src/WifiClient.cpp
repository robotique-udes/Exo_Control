#include "Wificlient.h"
#include "WifiServer.h"
#include <stdexcept>
#define UDP_PORT_SEND 4210


WifiClient::WifiClient() // Constructor
{

}


void WifiClient::wifiConnect() // Connect to Wi-Fi
{
    // IP address of the UDP server
    IPAddress watch_ip(192, 168, 4, 2);
    addIPAddress(watch_ip, EnumIPType::WATCH);

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
    IPAddress watch_ip(192, 168, 4, 2);//The get ip is broken
    UDP.beginPacket(watch_ip, UDP_PORT_SEND);
    UDP.write(data, data_lenght);
    UDP.endPacket();
    Serial.printf("UDP sent packet contents: %s\n", data);
}

void WifiClient::receiveMessage(unsigned char data[]) // Receive message from server
{
  unsigned char incomingPacket[255];
  int packetSize = UDP.parsePacket();
  if (packetSize) {
      int len = UDP.read(incomingPacket, 255);
      if (len > 0) {
          incomingPacket[len] = 0;
      }
      Serial.printf("UDP recieved packet contents: %s\n", incomingPacket);
  }
  data = incomingPacket;
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
    // Send connection request
    unsigned char connection_request[] = "Connection request";
    MessageBuilder message = MessageBuilder();
    message.add(connection_request);
    

    // Receive IP addresses
    unsigned char IPs[255];
    receiveMessage(IPs);
    deserializeMessage(IPs);    

    // Set IP addresses (CAN BE CHANGED)
    std::pair<std::string, int> key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(EnumIPType::WATCH));
    std::string watch_ip = dataMap[key];
    // convert string to const char*
    const char* watch_ip_char = watch_ip.c_str();
    // convert string to IP address
    IPAddress watch_ip_address;
    watch_ip_address.fromString(watch_ip_char);
    IPsList[0] = watch_ip_address;

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

IPAddress WifiClient::getIP(EnumIPType address) // Get IP address from list
{
    return IPsList[(int)address];
}

void WifiClient::deserializeMessage(unsigned char message[])
{
    Serial.println("Data Map updated.");

    // deserialize message into a map
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    deserializeJson(doc, message);

    // extract data from message
    std::pair<std::string, int> key;
    std::string value;

    // log
    JsonArray log = doc["logs"];
    serializeJson(log, Serial);
    Serial.println("\n\n LOGS \n\n");

    JsonArray logs = doc["logs"];
    for (int i = 0; i < logs.size(); i++)
    {
        key = std::make_pair("logs", static_cast<int>(logs[i]["ID"]));
        value = logs[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        dataMap[key] = value;
    }

    // bnoAngles
    JsonArray bnoAngles = doc["bnoAngles"];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(bnoAngles[i]["ID"]));
        value = bnoAngles[i]["value"].as<std::string>();
        dataMap[key] = value;
    }
    // bnoPositions
    JsonArray bnoPositions = doc["bnoPositions"];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(bnoPositions[i]["ID"]));
        value = bnoPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }
    // motorPositions
    JsonArray motorPositions = doc["motorPositions"];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(motorPositions[i]["ID"]));
        value = motorPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }
    // IP addresses
    JsonArray ipAddresses = doc["ipAddresses"];
    for (int i = 0; i < ipAddresses.size(); i++)
    {
        key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(ipAddresses[i]["ID"]));
        value = ipAddresses[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

}


