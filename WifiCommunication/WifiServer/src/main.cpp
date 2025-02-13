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

  wifiserver = WifiServer::GetInstance();
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

  WifiServer::upDate();

  // if(test && compteur >5)
  // {
  //   test = false;
  //   TestDeserializeData();
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
  float exoBack = 0;

  if(wifiserver->retrieveInformation(EnumBnoAngle::EXO_BACK, &exoBack) >= 0)
  {
    Serial.print("exoback: ");
    Serial.println(exoBack);
  }
  else
  {
    Serial.println("No value found for EXO_BACK");
  }

  if(wifiserver->retrieveInformation(EnumBnoAngle::HIP_L, &exoBack) >= 0)
  {
    Serial.print("HIP_L: ");
    Serial.println(exoBack);
  }
  else
  {
    Serial.println("No value found for HIP_L");
  }

  Serial.println("\n\n\n\ngoing to make the IP adresse");

  MessageBuilder message2 = MessageBuilder();
  IPAddress ipAdd(192, 168, 4, 3);
  message2.add(EnumIPType::EXOSKELETON, &ipAdd);
  Serial.println("Going to build the message");
  length = message2.buildHandshake();
  Serial.println("Build message finished");
  wifiserver->deserializeMessage(message2.getMessage(), length);
  Serial.print("Finished");
}