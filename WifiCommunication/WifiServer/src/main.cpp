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
    // wifiserver->DoAFlip();
    MessageBuilder message;
  }

  // Check if data is available
  int length = wifiserver->DataAvailable();
  if (length > 0) 
  {  
    wifiserver->ReadData(length);
    wifiserver->SendData(wifiserver->lastMessage, wifiserver->lastMessageLength);
  }

  delay(1);
}


void TestSendConnectedPeople()
{

}


// #include <WiFi.h>
// #include "esp_wifi.h"
// void setup()
// {
//   Serial.begin(115200);
//   WiFi.softAP("MyESP32AP"); 
// }

// void loop() {
//   wifi_sta_list_t wifi_sta_list;
//   tcpip_adapter_sta_list_t adapter_sta_list;
//   memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
//   memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
//   esp_wifi_ap_get_sta_list(&wifi_sta_list);
//   tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
//   for (int i = 0; i < adapter_sta_list.num; i++) 
//   {
//     tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
//     Serial.print("station nr ");
//     Serial.println(i);
//     Serial.print("MAC: ");
//     for(int i = 0; i< 6; i++)
//     {
//       Serial.printf("%02X", station.mac[i]);
//       if(i<5)
//         Serial.print(":");
//     }
    
//     ip4_addr_t addresse;
//     addresse.addr = station.ip.addr; 
//     Serial.print("\nIP: ");
//     Serial.println(ip4addr_ntoa(&(addresse)));
//   } 
//   Serial.println("-----------");  delay(5000); 
// }
