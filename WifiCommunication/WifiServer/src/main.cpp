// #include <WiFi.h>
// #include <WiFiUdp.h>
// #define UDP_PORT_RECEIVE 4210
#include"Arduino.h"
#include"WifiServer.h"
#include "MessageBuilder/MessageBuilder.h"
#include "enums.h"

void TestDeserializeData();

WifiServer* wifiserver;

unsigned long previousMillis = 0; // Stores the last time a message was printed
const unsigned long interval = 1000; // Interval between prints (1 second)

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);

  wifiserver = WifiServer::GetInstance("helloIAmUnder", "ItsTricky");
  wifiserver->Initialise();  
}

bool test = true;
int compteur = 0;

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    compteur++;
    Serial.print(compteur);
    Serial.println("   1 second has pass");
  }

  if(test && compteur >5)
  {
    test = false;
    TestDeserializeData();
  }

  // if(wifiserver->readyToSendHandShake)
  // {
  //   wifiserver->readyToSendHandShake = 0;
  //   wifiserver->DoAFlip();
  // }

  // // Check if data is available
  // int length = wifiserver->DataAvailable();
  // if (length > 0) 
  // {  
  //   wifiserver->ReadData(length);
  //   wifiserver->SendData(wifiserver->lastMessage, wifiserver->lastMessageLength);
  //   // float val;
  //   // wifiserver->retrieveInformation(EnumBnoAngle::EXO_BACK, &val);
  //   // Serial.printf("valeur obtenu: %2f", val);
  // }

  delay(1);
}


void TestDeserializeData()
{
  MessageBuilder message = MessageBuilder();
  unsigned char confirmation[16] = "this is my logs";
  message.add(confirmation);
  message.add(EnumBnoAngle::EXO_BACK, 42.5);
  message.add(EnumBnoPosition::THIGH_L, 78.5);
  Serial.println("Going to build the message");
  int length = message.buildMessage();
  Serial.println("Build message finished");
  wifiserver->deserializeMessage(message.getMessage(), length);

}