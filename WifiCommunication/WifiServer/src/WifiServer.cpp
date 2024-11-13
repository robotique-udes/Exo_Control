
#include"WifiServer.h"

 WifiServer:: WifiServer(const char* ssid, const char* passphrase)
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

int WifiServer::Initialise()
{
  if (!WiFi.softAP(ServerSSID, ServerPassword)) {
    log_e("Soft AP creation failed.");
    return -1;
  }
    WiFi.softAPConfig(Local_ip, Gateway, Subnet);
    MyIP = WiFi.softAPIP();
    
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