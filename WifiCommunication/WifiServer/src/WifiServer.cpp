
#include"WifiServer.h"
#include <esp_wifi.h>

WifiServer* WifiServer::wifiInstance = nullptr;

WifiServer* WifiServer::GetInstance(char* ssid, char* passphrase)
{
  if(wifiInstance == nullptr)
  {
    wifiInstance = new WifiServer(ssid, passphrase);
  }

  return wifiInstance;
}

WifiServer:: WifiServer(char* ssid, char* passphrase)
{
    ServerSSID = ssid;
    ServerPassword = passphrase;

    IPAddress local_ip(192, 168, 4, 2);
    Local_ip = local_ip;

    IPAddress gateway(192, 168, 4, 1);
    Gateway = gateway;
    IPAddress subnet(255, 255, 255, 0);
    Subnet = subnet;
    IPAddress myIP;
    MyIP = myIP;
}

void WiFiStationConnected(arduino_event_id_t event, arduino_event_info_t info) {
    Serial.println("Device connected to the access point!");

    //Fonctionne pas, fait cracher le esp32-s3
    
    // Get the list of connected devices
    // wifi_sta_list_t stationList;
    // tcpip_adapter_sta_list_t adapterList;
    // Serial.println("Initialize variables");

    // if (esp_wifi_ap_get_sta_list(&stationList) == ESP_OK)
    // {
    //   Serial.println("esp_wifi_ap_get_sta_list is ok");

    //   if(tcpip_adapter_get_sta_list(&stationList, &adapterList) == ESP_OK)
    //   {
    //     for (int i = 0; i < adapterList.num; i++) {
    //         Serial.printf("Device %d IP: %s\n", i + 1, adapterList.sta[i].ip);
    //     }

    //   }
    //   else
    //   {
    //     Serial.println("Esp tcpip is not ok");
    //   }
    // }
    // else
    // {
    //   Serial.println("Esp is not ok");
    // }
}

void WiFiStationAssignation(arduino_event_id_t event, arduino_event_info_t info){
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  WifiServer* wifiserver = WifiServer::GetInstance("helloIAmUnder", "ItsTricky");
  //Will need to check for only new people connected
  for (int i = 0; i < adapter_sta_list.num; i++) 
  {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    Serial.print("station nr ");
    Serial.println(i);
    Serial.print("MAC: ");
    
    for(int j = 0; j< 6; j++)
    { //Adding the users info
      Serial.printf("%02X", station.mac[j]);
      wifiserver->IPsList[i].mac[j] = station.mac[j];
      if(j<5)
        Serial.print(":");
    }
    
    ip4_addr_t addresse;
    addresse.addr = station.ip.addr; 
    Serial.print("\nIP: ");
    Serial.println(ip4addr_ntoa(&(addresse)));

    //Adding the users info
    wifiserver->IPsList[i].ipAdresse = IPAddress(addresse.addr);
    wifiserver->IPsList[i].ipType = EnumIPType::NONE;
  } 
  
  wifiserver->numClient = adapter_sta_list.num;

}

void WifiServer::newClientConnection(IpTypeList newClient)
{
  // IPsList[numClient] = newClient;
  // numClient++;
}

void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Station connected IP Add = ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr)); 
  Serial.println();
}

void WiFiStationDisconnected(arduino_event_id_t event, arduino_event_info_t info) {
    Serial.println("Device disconnected from the access point!");
}

void WifiServer::InitialiseIPList()
{
  numClient = 0;
  for(int i = 0; i < IP_LIST_SIZE; i++)
  {
    for(int j = 0; j < 6; j++)
    {
      IPsList[i].mac[j] = 0;
    }
    
    IPsList[i].ipType = EnumIPType::NONE;
    IPsList[i].ipAdresse = IPAddress(0,0,0,0);
  }
}

int WifiServer::Initialise()
{
  if (!WiFi.softAP(ServerSSID, ServerPassword)) 
  {
    log_e("Soft AP creation failed.");
    return -1;
  }

  WiFi.softAPConfig(Local_ip, Gateway, Subnet);
  MyIP = WiFi.softAPIP();    

  WiFi.onEvent(WiFiStationConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
  WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
  WiFi.onEvent(WiFiStationAssignation, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);

  Serial.print("AP IP address: ");
  Serial.println(MyIP);

  // Begin listening to UDP port
  UDP.begin(UDP_PORT_RECEIVE);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT_RECEIVE);

  return 0;
}

/**
 * @brief La fonction retourne le nombre de data à lire
 * 
 * @return true 
 * @return false 
 */
int WifiServer::DataAvailable()
{
  int longueur = UDP.parsePacket();
  //Serial.println(longueur);
    return longueur;
}

int WifiServer::ReadData(int length)
{
  unsigned char packet[200];
  int len = UDP.read(packet, length);
  lastMessageLength = len;
  if (len > 0)
  {
    packet[len] = '\0';
  }

// Get the sender's IP and port
  IPAddress senderIP = UDP.remoteIP();
  int senderPort = UDP.remotePort();

  Serial.print("Packet received from IP: ");
  Serial.println(senderIP);
  Serial.print("Port: ");
  Serial.println(senderPort);

  Serial.print("Packet received: ");

  for(int i = 0; i < length; i++)
  {
    lastMessage[i] = (char)packet[i];
    Serial.print((char)packet[i]);
  }
  Serial.println("\n");

  return length;
}

int WifiServer::SendData(unsigned char * packet, int length)
{
        // Send return packet
    Serial.print("UDP.remoteIP(): ");
    Serial.println(UDP.remoteIP());

    Serial.print("UDP.remotePort(): ");
    Serial.println(UDP.remotePort());


    Serial.print("myIP: ");
    Serial.println(MyIP);

    UDP.beginPacket(UDP.remoteIP(), 4211);
    Serial.print("Sending packet to ");
    Serial.print(UDP.remoteIP());
    Serial.print(" on port ");
    Serial.print(4211);
    Serial.print(":  size: ");
    Serial.print(length);
    Serial.print("\n\n");

    for(int byte = 0; byte < length; byte++)
    {
        Serial.print((char)packet[byte]);
    }
    Serial.println("");

    UDP.write(packet, length);
    UDP.endPacket();
    //Serial.println("Sent :)");
    return 0;
}

 WifiServer::~ WifiServer()
{
}

void WifiServer::handShake()
{
    // Send list of IPs
    MessageBuilder message;
    for(int i = 0; i < numClient; i++)
    {
      message.add(IPsList->ipType, IPsList->ipAdresse);
    }

    message.buildHandshake();
    unsigned char* mess = message.getMessage();
    SendData(mess, 20);//Probablement pas la bonne chose pour get le length, il faudra checker quoi faire
}

IPAddress WifiServer::getIP(EnumIPType index)
{
  for (int i = 0; i < IP_LIST_SIZE; i++)
  {
    if(IPsList[i].ipType == index)
      return IPsList[i].ipAdresse;
  }
  
  return nullptr;
}
