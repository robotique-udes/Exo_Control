// #include <WiFi.h>
// #include <WiFiUdp.h>
// #define UDP_PORT_RECEIVE 4210
#include"Arduino.h"
#include"WifiServer.h"
#include "MessageBuilder/MessageBuilder.h"
#include "enums.h"


WifiServer* wifiserver;

unsigned long previousMillis = 0; // Stores the last time a message was printed
const unsigned long interval = 1000; // Interval between prints (1 second)

unsigned char allo[4] = {'a','l','l','o'};

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);

  wifiserver = WifiServer::GetInstance("helloIAmUnder", "ItsTricky");
  wifiserver->Initialise();  
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("1 second has pass");
  }

  if(wifiserver->readyToSendHandShake)
  {
    wifiserver->readyToSendHandShake = 0;
    wifiserver->DoAFlip();
  }

  // Check if data is available
  int length = wifiserver->DataAvailable();
  if (length > 0) 
  {  
    wifiserver->ReadData(length);
    wifiserver->SendData(wifiserver->lastMessage, wifiserver->lastMessageLength);
    float val;
    wifiserver->retrieveInformation(EnumBnoAngle::EXO_BACK, &val);
    Serial.printf("valeur obtenu: %2f", val);
  }

  delay(1);
}


void TestSendConnectedPeople()
{

}