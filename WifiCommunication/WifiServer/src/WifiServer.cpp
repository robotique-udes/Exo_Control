
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
    this->IPsList[0].ipAdresse = local_ip;
    this->IPsList[0].ipType = EnumIPType::WATCH;
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
  for (int i = 0; i < adapter_sta_list.num && i < IP_LIST_SIZE; i++) 
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
    wifiserver->IPsList[i+1].ipAdresse = IPAddress(addresse.addr);
    wifiserver->IPsList[i+1].ipAdd = addresse;
    wifiserver->IPsList[i+1].ipType = EnumIPType::UNKNOWN_TYPE;
  } 

  wifiserver->readyToSendHandShake = 1;
  wifiserver->numClient = adapter_sta_list.num + 1;
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
    wifiserver->numClient--;
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
  unsigned char packet[800];
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

  // Deserialize the message
  deserializeMessage(packet, length);

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

    UDP.beginPacket(IPAddress(192, 168, 4, 3), 4211); // À RENDRE MODULABLE
    Serial.print("Sending packet to ");
    Serial.print(test);
    Serial.print(" on port ");
    Serial.print(UDP.remotePort());
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
    unsigned char connection_request[] = "Connection request";
    MessageBuilder message = MessageBuilder();
    message.add(connection_request);
    Serial.println("Starting the handShake");
    for(int i = 0; i < numClient; i++)
    {
      Serial.print(i);
      Serial.print(" NumClient: ");
      Serial.println(numClient);

      Serial.println((int)IPsList[i].ipType);
      Serial.println(IPsList[i].ipAdresse);
      //message.add(IPsList[i].ipType, IPsList[i].ipAdd.addr);
      message.add(IPsList[i].ipType, &(IPsList[i].ipAdresse));
    }

    int length = message.buildHandshake();
    unsigned char* mess = message.getMessage();
    delay(1000); // À CHANGER
    SendData(mess, length);
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

void WifiServer::DoAFlip()
{
  this->handShake();
  Serial.println("Finished hand shake");
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
      IPAddress val = IPAddress();
      val.fromString(unifiedMap[key].c_str());
      *value = val;
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
      delay(1000);
      wifiserver->readyToSendHandShake = 0;
      wifiserver->handShake();
    }
  }
}

void WifiServer::deserializeMessage(unsigned char message[], int length)
{
    Serial.println("Unified Map updated.");
    Serial.println(length);
    for(int byte = 0; byte < length; byte++)
    {
        Serial.print((char)message[byte]);
    }
    Serial.println("");

    // deserialize message into a map
    DynamicJsonDocument doc(length);
    deserializeJson(doc, message);

    // extract data from message
    std::pair<std::string, int> key;
    std::string value;

    // log
    JsonArray log = doc["logs"];
    serializeJson(log, Serial);
    Serial.println("\n\n LOGS \n\n");

    JsonArray logs = doc["logs"];
    for (int i = 0; i < logs.size(); i++)
    {
        key = std::make_pair("logs", static_cast<int>(logs[i]["ID"]));
        value = logs[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        unifiedMap[key] = value;
    }

    // bnoAngles
    JsonArray bnoAngles = doc["bnoAngles"];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(bnoAngles[i]["ID"]));
        value = bnoAngles[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        unifiedMap[key] = value;
    }

    // bnoPositions
    JsonArray bnoPositions = doc["bnoPositions"];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(bnoPositions[i]["ID"]));
        value = bnoPositions[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // motorPositions
    JsonArray motorPositions = doc["motorPositions"];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(motorPositions[i]["ID"]));
        value = motorPositions[i]["value"].as<std::string>();
        unifiedMap[key] = value;
    }

    // IP addresses
    JsonArray ipAddresses = doc["ipAddresses"];
    for (int i = 0; i < ipAddresses.size(); i++)
    {
        key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(ipAddresses[i]["ID"]));
        value = ipAddresses[i]["value"].as<std::string>();
        unifiedMap[key] = value;
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