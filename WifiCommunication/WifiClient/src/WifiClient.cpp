#include <WiFiClient.h>
#include <String.h>

#define UDP_PORT_SEND 4210


WifiClient::WifiClient() // Constructor
{
  // WiFi credentials
  const char* ssid = "helloIAmUnder";  // WiFi network name
  const char* password = "ItsTricky"; // WiFi network password

  // Connect to Wi-Fi
  // connect();
}


void WifiClient::connect() // Connect to Wi-Fi
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (!isConnected()) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // IP address of the UDP server
    IPAddress server_ip(192, 168, 1, 2);
    serverIP = server_ip;

    // Set up UDP
    UDP.begin(localUdpPort);
    Serial.println("UDP client started");
}

void WifiClient::disconnect() // Disconnect from Wi-Fi
{
  WiFi.disconnect();
  Serial.println("Disconnected from WiFi.");
}


void WifiClient::sendMessage(unsigned char data[]) // Send message to server
{
  UDP.beginPacket(serverIP, UDP_PORT_SEND);
  UDP.write(data, 22);
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


