#include "WifiClient.h"

WifiClient client;

void setup() {
    // Start the Serial communication
    // Serial.begin(115200)
    client.wifiConnect();
    
}

void loop() {
    String message = "Hello there, my friend";
    unsigned char table[22];

    for (size_t i = 0; i < 22; i++)
    {
        table[i] = message[i];
    }

    // Send the message
    client.sendMessage(table, EnumIPType::WATCH);

    // Wait for 2 second before sending the next message
    delay(2000);
}



//1- On commence par recevoir du serveur les personnes qui sont accessible
//2- Il faut envoyer qui on est apres


void FonctionTest1Communication()
{
    //Receive 
    //then send

    
}
