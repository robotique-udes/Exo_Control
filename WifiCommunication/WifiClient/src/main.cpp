#include <WiFi.h>
#include <WiFiUdp.h>

#define UDP_PORT_SEND 4210

// WiFi credentials
const char* ssid = "MyWifi";  // Replace with your WiFi network name
const char* password = "123456789";  // Replace with your WiFi network password

// UDP
WiFiUDP UDP;
IPAddress serverIP(192, 168, 4, 2); // IP address of the UDP server
unsigned int localUdpPort = 4211;  // Local port to receive responses (if needed)

void setup() {
  // Start the Serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Start UDP
  UDP.begin(localUdpPort);
  Serial.println("UDP client started");
}

void loop() {
  String message = "Hello there, my friend";
  unsigned char table[25];

  for (size_t i = 0; i < 20; i++)
  {
    table[i] = message[i];
  }
  
  // Send message to server
  UDP.beginPacket(serverIP, UDP_PORT_SEND);
  UDP.write(table, 20);
  UDP.endPacket();

  Serial.println("Message sent: Hello there, my friend");

  // Wait for 1 second before sending the next message
  delay(2000);
}
