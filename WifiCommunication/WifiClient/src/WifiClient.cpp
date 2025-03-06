#include "Wificlient.h"
#include "WifiServer.h"
#include <stdexcept>
#define UDP_PORT_SEND 4210

WifiClient* WifiClient::clientInstance = nullptr;

WifiClient* WifiClient::GetInstance()
{
  if(clientInstance == nullptr)
  {
    clientInstance = new WifiClient();
  }

  return clientInstance;
}

WifiClient::WifiClient() // Constructor
{
    
}


void WifiClient::wifiConnect() // Connect to Wi-Fi
{
    static unsigned long previousMillisConnected =  millis(); // coundown to connect to wifi
    static unsigned long previousMillisDot =  millis(); // Stores the last time a dot was printed
    unsigned long currentMillis =  millis(); // Stores the current time

    // WiFi mode
    WiFi.mode(WIFI_STA);

    // Try to connect for 5 seconds
    WiFi.begin(ssid, password);
    Serial.printf("Attempting to connect to \"%s\" with password \"%s\".\n\n", ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        currentMillis = millis();
        if (currentMillis - previousMillisDot >= 1000)
        {
            Serial.print(".");
            previousMillisDot = currentMillis;
        }
        else if (currentMillis - previousMillisConnected >= 5000)
        {
            Serial.println("Failed to connect to WiFi.");
            return;
        }
    }
    Serial.println("Connected to WiFi!");

    // Set up UDP
    UDP.begin(localUdpPort);
    Serial.println("UDP client started");

    // Handshake with server
    handShake();
}

void WifiClient::wifiDisconnect() // Disconnect from Wi-Fi
{
    if (!isConnected()) 
    {
        Serial.println("Client not connected to wifi server.");
        return;
    }
    WiFi.disconnect();
    Serial.println("Client disconnected from the wifi server.");
}


void WifiClient::sendMessage(int data_lenght, unsigned char data[], EnumIPType address) // Send message to server
{
    if (!isConnected()) 
    {
        Serial.println("Client not connected to wifi server.");
        return;
    }
    IPAddress sendingAddress;
    sendingAddress.fromString(IPsList[(int)EnumIPType::WATCH].c_str());
    UDP.beginPacket(sendingAddress, UDP_PORT_SEND);
    Serial.println("Sending message to server...");
    for (int i = 0; i < data_lenght; i++)
    {
        Serial.print((char)data[i]);
    }
    Serial.println();
    UDP.write(data, data_lenght);
    UDP.endPacket();
    Serial.printf("UDP sent packet contents: %s\n", data);
}

int WifiClient::dataAvailable() // Check if data is available
{
    return lenght_message_recieved = UDP.parsePacket();
}

void WifiClient::receiveMessage() // Receive message from server
{
    if (!isConnected()) 
    {
        Serial.println("Client not connected to wifi server.");
        return;
    }
    unsigned char incomingPacket[lenght_message_recieved + 1];
    if (lenght_message_recieved) 
    {
        int len = UDP.read(incomingPacket, lenght_message_recieved);
        if (len > 0) {
            incomingPacket[len] = 0;
        }
        Serial.printf("UDP recieved packet contents: %s\n", incomingPacket);

        for(int i = 0; i < lenght_message_recieved; i++)
        {
            message_recieved[i] = incomingPacket[i];
        }
    }

    deserializeMessage(incomingPacket, lenght_message_recieved);

}

bool WifiClient::isConnected() // Check if connected to Wi-Fi
{
    return WiFi.status() == WL_CONNECTED;
}

void WifiClient::wifiOff() // Turn off Wi-Fi to save power
{
    WifiClient::wifiDisconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi turned off.");
}

void WifiClient::wifiOn() // Turn on Wi-Fi
{
    WiFi.mode(WIFI_STA);
    WifiClient::wifiConnect();
    Serial.println("\nConnected to WiFi!");
}

void WifiClient::handShake() // Handshake with server
{   

    // Receive IP addresses
    int longueur_ips = dataAvailable();
    if (!longueur_ips) 
    {
        Serial.print("HandShake - Waiting for IP addresses... lentgh recieved : ");
        Serial.println(longueur_ips); 
        delay(800);
        return;
    }
    unsigned char IPs[longueur_ips];
    receiveMessage();
    deserializeMessage(message_recieved, longueur_ips);

    // Set IP addresses (CAN BE CHANGED)
    std::pair<std::string, int> key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(EnumIPType::WATCH));
    std::string watch_ip = dataMap[key];
    IPsList[(int)EnumIPType::WATCH] = watch_ip;

    //Set IP Exo
    key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(EnumIPType::UNKNOWN_TYPE));
    std::string exo_ip = dataMap[key];

    IPsList[(int)EnumIPType::EXOSKELETON] = exo_ip;

    // convert string to const char*
    const char* watch_ip_char = watch_ip.c_str();

    // print IP address input
    Serial.print("Watch IP dataMap: ");
    Serial.print(watch_ip_char);
    Serial.println(".");

    // convert const char* to IPAddress
    IPAddress watch_ip_address;
    watch_ip_address.fromString(watch_ip_char);

    // Send connection confirmation
    const char confirmation[22] = "Connection confirmed";
    MessageBuilder message = MessageBuilder();
    message.add(confirmation);
    Serial.println("Starting the handShake");
    IPAddress test;
    test.fromString(IPsList[(int)EnumIPType::EXOSKELETON].c_str());
    message.add(EnumIPType::EXOSKELETON, &test);

    int length = message.buildHandshake();
    unsigned char* mess = message.getMessage();
    sendMessage(length, mess, EnumIPType::WATCH);

    handShakeDone = true;
}

void WifiClient::addIPAddress(IPAddress ip, EnumIPType ID) // Add IP address to list
{
    int index = (int)ID;
    IPsList[index] = ip;
    IPsListSize++;
}

std::string WifiClient::getIP(EnumIPType address) // Get IP address from list
{
    return IPsList[(int)address];
}

void WifiClient::deserializeMessage(unsigned char message[], int length)
{
    Serial.println("Data Map update");

    // deserialize message into a map
    JsonDocument doc;
    deserializeJson(doc, message);
    //serializeJson(doc, Serial);

    // extract data from message
    std::pair<std::string, int> key;
    std::string value;

    // logs
    JsonArray logs = doc[NESTED_LOGS];
    for (int i = 0; i < logs.size(); i++)
    {
        key = std::make_pair("logs", static_cast<int>(logs[i]["ID"]));
        value = logs[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        dataMap[key] = value;
    }

    // bnoAngles
    JsonArray bnoAngles = doc[NESTED_BNO_ANGLES];
    for (int i = 0; i < bnoAngles.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_ANGLE, static_cast<int>(bnoAngles[i]["ID"]));
        value = bnoAngles[i]["value"].as<std::string>();
        Serial.print("Key: ");
        Serial.print(key.first.c_str());
        dataMap[key] = value;
    }

    // bnoPositions
    JsonArray bnoPositions = doc[NESTED_BNO_POSITIONS];
    for (int i = 0; i < bnoPositions.size(); i++)
    {
        key = std::make_pair(ENUM_BNO_POSITION, static_cast<int>(bnoPositions[i]["ID"]));
        value = bnoPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

    // motorPositions
    JsonArray motorPositions = doc[NESTED_BNO_MOTEUR];
    for (int i = 0; i < motorPositions.size(); i++)
    {
        key = std::make_pair(ENUM_MOTOR_POSITION, static_cast<int>(motorPositions[i]["ID"]));
        value = motorPositions[i]["value"].as<std::string>();
        dataMap[key] = value;
    }

    // IP addresses
    JsonArray IPs = doc[NESTED_IP_TYPE];
    Serial.print(IPs.size());
    Serial.println(" IP List:");
    for (int i = 0; i < IPs.size(); i++)
    {
        key = std::make_pair(ENUM_IP_TYPE, static_cast<int>(IPs[i]["ID"]));
        value = IPs[i]["value"].as<std::string>();
        Serial.print("Value: ");
        Serial.println(value.c_str());
        dataMap[key] = value;
    }

    Serial.println("Contents of the map:");
    for (const auto& entry : dataMap) {
        Serial.print("Key: (");
        Serial.print(entry.first.first.c_str());
        Serial.print(", ");
        Serial.print(entry.first.second);
        Serial.print(") -> Value: ");
        Serial.println(entry.second.c_str());
    }
    Serial.println(".");
}

void WifiClient::upDate()
{
    // Connect to the wifi server automatically, if not connected, then sends a handshake.
    // Must be called in a loop, checks if there's a new message from the server every 2 seconds.
    WifiClient* wificlient = WifiClient::GetInstance();
    static unsigned long previousMillis =  millis();
    unsigned long currentTime = millis();

    if (!wificlient->isConnected()) 
    {
        wificlient->wifiConnect();
    }
    else if (!wificlient->isConnected() && !wificlient->handShakeDone)
    {
        wificlient->handShake();
    }
    else if (currentTime - previousMillis >= 2000)
    {
        wificlient->dataAvailable();
        if (wificlient->lenght_message_recieved > 0)
        {
            wificlient->receiveMessage();
        }
        previousMillis = currentTime;
    }
}

WifiClient::~ WifiClient()
{
}

