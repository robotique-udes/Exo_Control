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


void WifiClient::sendMessage(unsigned char data[], EnumIPType address) // Send message to server
{
  UDP.beginPacket(getIP(address), UDP_PORT_SEND);
  // find size of data
  int size = 0;
  while (true)
  {
    try 
    {
        if (data[size] == '\0')
        {
            break;
        }

    } 
    catch (const std::exception& e) 
    {
        throw new std::range_error("Data not containing a null character.");
    }
    size++;
  } 

  UDP.write(data, size);
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

    // Set IP addresses
    IPsList[0] = dataMap[std::make_pair('ip', 0)];

    // Send connection confirmation
    unsigned char confirmation[22] = "Connection confirmed";
    sendMessage(confirmation, EnumIPType::WATCH);

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
    // deserialize message into a map
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    deserializeJson(doc, message);

    std::map<std::pair<unsigned char, int>, unsigned char> dataMap;
    // log
    JsonArray log = doc["logs"];
    dataMap[std::make_pair('log', 0)] = log[0];
    // bnoAngles
    JsonArray bnoAngles = doc["bnoAngles"];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        dataMap[std::make_pair('ba', bnoAngles[i]["ID"].as<int>())] = bnoAngles[i]["value"];
    }
    // bnoPositions
    JsonArray bnoPositions = doc["bnoPositions"];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        dataMap[std::make_pair('bp', bnoPositions[i]["ID"].as<int>())] = bnoPositions[i]["value"];
    }
    // motorPositions
    JsonArray motorPositions = doc["motorPositions"];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        dataMap[std::make_pair('mp', motorPositions[i]["ID"].as<int>())] = motorPositions[i]["value"];
    }
    // IP addresses
    JsonArray ipAddresses = doc["ipAddresses"];
    for (int i = 0; i < ipAddresses.size(); i++)
    {
        dataMap[std::make_pair('ip', ipAddresses[i]["ID"].as<int>())] = ipAddresses[i]["value"];
    }

}


