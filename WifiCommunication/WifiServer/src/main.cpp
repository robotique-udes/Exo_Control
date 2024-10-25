#include <WiFi.h>
#include <WiFiUdp.h>
#define UDP_PORT_RECEIVE 4210

// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";

// WiFiServer server(80);
// WiFiClient client;
IPAddress local_ip(192, 168, 4, 2);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress myIP;

#define WIFI_SSID "MyWifi"
#define WIFI_PASSWORD "123456789"


/**
 * @brief Initialisation du serveur
 * N'importe qui peut se connecter au serveur avec le bon mot de passe
 * 
 * @return int 
 */
int interface_WIFI_initialise()
{
  if (!WiFi.softAP(WIFI_SSID, WIFI_PASSWORD)) {
    log_e("Soft AP creation failed.");
    return -1;
  }
    WiFi.softAPConfig(local_ip, gateway, subnet);
    myIP = WiFi.softAPIP();
    
  Serial.print("AP IP address: ");
  Serial.println(myIP);

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
int interface_WIFI_Data_Available()
{
  int longueur = UDP.parsePacket();
  Serial.println(longueur);
    return longueur;
}

int interface_WIFI_Read(int length)
{
  unsigned char packet[200];
  int len = UDP.read(packet, length);
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
      Serial.print((char)packet[i]);
  }
  Serial.println("\n");

  return length;
}

int interface_WIFI_Send(unsigned char * packet, int length)
{
        // Send return packet
    Serial.print("UDP.remoteIP(): ");
    Serial.println(UDP.remoteIP());

    Serial.print("UDP.remotePort(): ");
    Serial.println(UDP.remotePort());


    Serial.print("myIP: ");
    Serial.println(myIP);

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




unsigned char allo[4] = {'a','l','l','o'};

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);
  interface_WIFI_initialise();
  delay(3000);
  interface_WIFI_Send(allo, 4);
}

void loop() {

  // Check if data is available
  int length = interface_WIFI_Data_Available();
  if (length > 0) 
  {  
    interface_WIFI_Read(length);
    interface_WIFI_Send(allo, 4);
  }

  delay(1000);
}
