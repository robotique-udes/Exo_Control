// #include <WiFi.h>
// #include <WiFiUdp.h>
// #define UDP_PORT_RECEIVE 4210
#include"Arduino.h"
#include"WifiServer.h"
#include "MessageBuilder.h"


WifiServer wifiserver("helloIAmUnder", "ItsTricky");

unsigned long previousMillis = 0; // Stores the last time a message was printed
const unsigned long interval = 1000; // Interval between prints (1 second)

unsigned char allo[4] = {'a','l','l','o'};

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);
  wifiserver.Initialise();

  
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("1 second has pass");
  }

  // Check if data is available
  int length = wifiserver.DataAvailable();
  if (length > 0) 
  {  
    wifiserver.ReadData(length);
    wifiserver.SendData(wifiserver.lastMessage, wifiserver.lastMessageLength);
  }

  delay(1);
}


void TestSendConnectedPeople()
{
  
}