// #include <WiFi.h>
// #include <WiFiUdp.h>
// #define UDP_PORT_RECEIVE 4210
#include"Arduino.h"
#include"WifiServer.h"


WifiServer wifiserver("helloIAmUnder", "ItsTricky");


unsigned char allo[4] = {'a','l','l','o'};

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);
  wifiserver.Initialise();
}

void loop() {

  // Check if data is available
  int length = wifiserver.DataAvailable();
  if (length > 0) 
  {  
    wifiserver.ReadData(length);
    wifiserver.SendData(wifiserver.lastMessage, wifiserver.lastMessageLength);
  }

  delay(1);
}
