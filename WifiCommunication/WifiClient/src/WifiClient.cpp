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
  DynamicJsonDocument dataDoc = MessageBuilder().deserializeMessage(data);
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
    unsigned char IPs[255];
    receiveMessage(IPs);

    // Set IP addresses
    // TODO

    // Send connection confirmation
    unsigned char connection_confirmed[] = "Connection confirmed";
    MessageBuilder handshake = MessageBuilder();
    handshake.add(connection_confirmed);
    handshake.add(EnumIPType::WATCH, getIP(EnumIPType::WATCH));
    handshake.buildHandshake();
    sendMessage(handshake.getMessage(), EnumIPType::WATCH);

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


