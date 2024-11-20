#include <String.h>
#include "WiFiClient.h"
#include "enums.h"

#define UDP_PORT_SEND 4210


WifiClient::WifiClient() // Constructor
{

}


void WifiClient::wifiConnect() // Connect to Wi-Fi
{
    // IP address of the UDP server
    IPAddress watch_ip(192, 168, 4, 2);
    addIPAddress(watch_ip);

    // WiFi mode
    WiFi.mode(WIFI_STA);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.printf("Attempting to connect to \"%s\" with password \"%s\".\n\n", ssid, password);
    while (!isConnected()) {
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


void WifiClient::sendMessage(unsigned char data[], enum IPType address) // Send message to server
{
  UDP.beginPacket(getIP(address), UDP_PORT_SEND);
  UDP.write(data, 22); // TODO change 22 to size of data
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
    // Receive IP addresses
    unsigned char IPs[255];
    receiveMessage(IPs);

    // Set IP addresses
    // TODO

    // Send connection confirmation
    unsigned char confirmation[22] = "Connection confirmed";
    sendMessage(confirmation, IPType::WATCH);

}

void WifiClient::addIPAddress(IPAddress ip) // Add IP address to list
{
    IPsList[IPsListSize] = ip;
    IPsListSize++;
}

IPAddress WifiClient::getIP(enum IPType address) // Get IP address from list
{
    int index = (int)address;
    return IPsList[index];
}


