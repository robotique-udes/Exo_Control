
#include"WifiServer.h"
#include <esp_wifi.h>
#include <arduinoJson.h>

WifiServer* WifiServer::wifiInstance = nullptr;

WifiServer* WifiServer::GetInstance()
{
  if(wifiInstance == nullptr)
  {
    wifiInstance = new WifiServer(SSID, PASS_PHRASE);
  }

  return wifiInstance;
}

WifiServer:: WifiServer(char* ssid, char* passphrase)
{
    ServerSSID = ssid;
    ServerPassword = passphrase;
    readyToSendHandShake = 0;
    timerBeforeSendinghandShake = 0;


    IPAddress local_ip(192, 168, 4, 2);
    Local_ip = local_ip;

    IPAddress gateway(192, 168, 4, 1);
    Gateway = gateway;
    IPAddress subnet(255, 255, 255, 0);
    Subnet = subnet;
    IPAddress myIP;
    MyIP = myIP;

    //Adding the users info
    IpTypeList newClient = { EnumIPType::WATCH, local_ip, NULL};
    this->IPsListVector.push_back(newClient);
}

bool CheckMacAddressInList(WifiServer* wifiserver, uint8_t* mac, int *positionInList = nullptr)
{
  for(int i = 1; i < wifiserver->IPsListVector.size(); i++)//i=1 for the server info in the list[0]
  {
    bool perfectMatch = true;

    if(positionInList != nullptr)
      *positionInList = i;

    for(int j = 0; j < 6; j++)
    {
      if(wifiserver->IPsListVector.at(i).mac[j] != mac[j])
      {
        perfectMatch = false;
        break;
      }
    }

    if(perfectMatch == true)
      return true;
  }

  return false;
}

void WiFiStationConnected(arduino_event_id_t event, arduino_event_info_t info) {
    Serial.println("Device connected to the access point!");
}

void WiFiStationAssignation(arduino_event_id_t event, arduino_event_info_t info){
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  WifiServer* wifiserver = WifiServer::GetInstance();
  //Will need to check for only new people connected
  for (int i = 0; i < adapter_sta_list.num && i < wifiserver->IPsListVector.size(); i++) 
  {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    IpTypeList newClient;

    Serial.println("Checking in the list of connected clients");

    if(CheckMacAddressInList(wifiserver, station.mac))
    {
      Serial.print("MAC: ");
      for(int j = 0; j< 6; j++)
      {
        Serial.printf("%02X", station.mac[j]);
        if(j<5)
          Serial.print(":");
      }
      Serial.println("This user is already part of the list.");
      continue;
    }

    Serial.print("station nr ");
    Serial.println(i);
    Serial.print("MAC: ");
    
    for(int j = 0; j< 6; j++)
    { //Adding the users info
      Serial.printf("%02X", station.mac[j]);
      newClient.mac[j] = station.mac[j];
      if(j<5)
        Serial.print(":");
    }
    
    ip4_addr_t addresse;
    addresse.addr = station.ip.addr; 
    Serial.print("\nIP: ");
    Serial.println(ip4addr_ntoa(&(addresse)));

    //Adding the users info
    newClient.ipType = EnumIPType::UNKNOWN_TYPE;
    newClient.ipAdresse = IPAddress(addresse.addr);
    wifiserver->IPsListVector.push_back(newClient);
  } 

  wifiserver->readyToSendHandShake = 1;
}

void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Station connected IP Add = ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr)); 
  Serial.println();
}

void WiFiStationDisconnected(arduino_event_id_t event, arduino_event_info_t info) 
{
    Serial.println("Device disconnected from the access point!");
    WifiServer* wifiserver = WifiServer::GetInstance();

    uint8_t* mac = info.wifi_ap_stadisconnected.mac;

    for (int i = 1; i < wifiserver->IPsListVector.size(); i++) 
    {
      Serial.print("station nr ");
      Serial.println(i);
      Serial.print("MAC: ");
      
      for(int j = 0; j< 6; j++)
      { //Adding the users info
        Serial.printf("%02X", mac[j]);
        if(j<5)
          Serial.print(":");
      }

      int positionList = -1;
      if(CheckMacAddressInList(wifiserver, mac, &positionList))
      {
        Serial.println("This user disconnected and will be removed");

        if(positionList >= 0)
          wifiserver->IPsListVector.erase(wifiserver->IPsListVector.begin() + positionList);

        break;
      }
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
  return UDP.parsePacket();
}

int WifiServer::ReadData(int length)
{
  unsigned char packet[800];
  int len = UDP.read(packet, length);
  if (len > 0)
  {
    packet[len] = '\0';
  }

  Serial.println("Raw Data:");
  for(int i = 0; i < length; i++)
  {
    Serial.print((char)packet[i]);
  }

  deserializeMessage(packet, length);

  return length;
}

int WifiServer::SendData(unsigned char * packet, int length, IPAddress ipAddress)
{
  UDP.beginPacket(ipAddress, UDP_PORT_SEND);
  UDP.write(packet, length);
  UDP.endPacket();

  return 0;
}

int WifiServer::SendData(unsigned char * packet, int length, EnumIPType ipType)
{
  IPAddress address;
  int errorCode = this->retrieveInformation(ipType, &address);
  if(errorCode != 0)
  {
    return errorCode;
  }

  UDP.beginPacket(address, UDP_PORT_SEND);
  UDP.write(packet, length);
  UDP.endPacket();

  return 0;
}

 WifiServer::~ WifiServer()
{
}

void WifiServer::handShake()
{
  // Send list of IPs
  unsigned char connection_request[] = "Connection request";
  MessageBuilder message = MessageBuilder();
  message.add("Connection request");

  Serial.println("Starting the handShake");
  for(int i = 0; i < IPsListVector.size(); i++)
  {
    message.add(IPsListVector.at(i).ipType, &(IPsListVector.at(i).ipAdresse));
  }

  int length = message.buildHandshake();
  unsigned char* mess = message.getMessage();

  for(int j = 0; j < IPsListVector.size(); j++)
  {
    if(IPsListVector.at(j).ipType == EnumIPType::UNKNOWN_TYPE)
    {
      SendData(mess, length, IPsListVector.at(j).ipAdresse);
    }
  }
}

bool WifiServer::isExoConnected()
{
  for(int j = 0; j < IPsListVector.size(); j++)
  {
    if(IPsListVector.at(j).ipType == EnumIPType::EXOSKELETON)
    {
      return true;
    }
  }

  return false;
}

/// @brief Trying to find key in dictionary. Return value 0 == good, -1 failed conversion, -2 not found
/// @param BNO_NAME 
/// @param value 
/// @return 
int WifiServer::retrieveInformation(EnumBnoAngle BNO_NAME, float* value)
{
  auto key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(BNO_NAME));

  if (unifiedMap.find(key) != unifiedMap.end()) 
  {
    try
    {
      Serial.println(unifiedMap[key].c_str());
      *value = std::stof(unifiedMap[key].c_str());
    }
    catch(const std::invalid_argument &e)
    {
      Serial.println("Conversion to float failed...");
      return -1;
    }
       
    return 0;
  }

  Serial.println("Key not found!");
  return -2;  
}

int WifiServer::retrieveInformation(EnumBnoPosition BNO_NAME, float* value)
{
  auto key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(BNO_NAME));

  if (unifiedMap.find(key) != unifiedMap.end()) 
  {
    try
    {
      Serial.println(unifiedMap[key].c_str());
      *value = std::stof(unifiedMap[key].c_str());
    }
    catch(const std::invalid_argument &e)
    {
      Serial.println("Conversion to float failed...");
      return -1;
    }
       
    return 0;
  }

  Serial.println("Key not found!");
  return -2;  
}

int WifiServer::retrieveInformation(EnumMotorPosition MOTOR_NAME, float* value)
{
  auto key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(MOTOR_NAME));

  if (unifiedMap.find(key) != unifiedMap.end()) 
  {
    try
    {
      Serial.println(unifiedMap[key].c_str());
      *value = std::stof(unifiedMap[key].c_str());
    }
    catch(const std::invalid_argument &e)
    {
      Serial.println("Conversion to float failed...");
      return -1;
    }
       
    return 0;
  }

  Serial.println("Key not found!");
  return -2;  
}

int WifiServer::retrieveInformation(EnumIPType IP_NAME, IPAddress* value)
{
  auto key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(IP_NAME));

  if (unifiedMap.find(key) != unifiedMap.end()) 
  {
    try
    {
      Serial.println(unifiedMap[key].c_str());
      IPAddress newClient = IPAddress();
      newClient.fromString(unifiedMap[key].c_str());
      *value = newClient;
    }
    catch(const std::invalid_argument &e)
    {
      Serial.println("Conversion to float failed...");
      return -1;
    }
       
    return 0;
  }

  Serial.println("Key not found!");
  return -2;  
}

void WifiServer::upDate()
{
  static unsigned long previousMillis = 0; // Stores the last time a message was printed
  WifiServer* wifiserver = WifiServer::GetInstance();
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= INTERVAL_10ms)
  {
    previousMillis = currentMillis;

    int length = wifiserver->DataAvailable();//Check for new informations
    // Serial.print("Length data received: ");
    // Serial.println(length);
    if (length > 0) 
    {  
      wifiserver->ReadData(length);
      //update the values
    }

    if(wifiserver->readyToSendHandShake)//Send the handShakes
    {
      wifiserver->timerBeforeSendinghandShake++;

      if(wifiserver->timerBeforeSendinghandShake >= INTERVAL_BEFORE_SEND_HANDSHAKE)
      {
        wifiserver->timerBeforeSendinghandShake = 0;
        wifiserver->readyToSendHandShake = 0;
        wifiserver->handShake();
      }
    }
  }
}

void WifiServer::deserializeMessage(unsigned char message[], int length)
{
    // deserialize message into a map
    DynamicJsonDocument doc(length);
    deserializeJson(doc, message);
    Serial.println("Json: ");
    serializeJson(doc, Serial);

    Serial.println("\n\n");
    // extract data from message
    std::pair<std::string, int> key;
    std::string value;

    JsonArray logs = doc[NESTED_LOGS];
    for (int i = 0; i < logs.size(); i++)
    {
        key = std::make_pair("logs", static_cast<int>(logs[i]["ID"]));
        value = logs[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // bnoAngles
    JsonArray bnoAngles = doc[NESTED_BNO_ANGLES];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(bnoAngles[i]["ID"]));
        value = bnoAngles[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // bnoPositions
    JsonArray bnoPositions = doc[NESTED_BNO_POSITIONS];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(bnoPositions[i]["ID"]));
        value = bnoPositions[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // motorPositions
    JsonArray motorPositions = doc[NESTED_BNO_MOTEUR];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(motorPositions[i]["ID"]));
        value = motorPositions[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // informations
    JsonArray infos = doc[NESTED_INFORMATIONS];
    for (int i = 0; i < infos.size(); i++)
    {
        key = std::make_pair(ENUM_INFO_TYPE, static_cast<int>(infos[i]["ID"]));
        value = infos[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // IP addresses
    JsonArray ipAddresses = doc[NESTED_IP_TYPE];
    for (int i = 0; i < ipAddresses.size(); i++)
    {
      key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(ipAddresses[i]["ID"]));
      value = ipAddresses[i]["value"].as<std::string>();
      unifiedMap[key] = value;

      for(int j = 0; j < IPsListVector.size(); j++)
      {
        if(IPsListVector.at(j).ipType == EnumIPType::UNKNOWN_TYPE && IPsListVector.at(j).ipAdresse.toString() == value.c_str())
        {
          IPsListVector.at(j).ipType = static_cast<EnumIPType>(static_cast<int>(ipAddresses[i]["ID"]));
        }
      }
    }

    Serial.println("Contents of the map:");
    for (const auto& entry : unifiedMap) {
        Serial.print("Key: (");
        Serial.print(entry.first.first.c_str());
        Serial.print(", ");
        Serial.print(entry.first.second);
        Serial.print(") -> Value: ");
        Serial.println(entry.second.c_str());
    }
}