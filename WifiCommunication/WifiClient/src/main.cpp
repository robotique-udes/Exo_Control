#include "WifiClient.h"
#include "MessageBuilder/MessageBuilder.h"

WifiClient client;

void setup() {
    // Start the Serial communication
    Serial.begin(115200);
    
}

void loop() {
    // Connect the client after 5 seconds
    unsigned long time = millis();
    static unsigned long time_passed_init = millis();
    static unsigned long time_passed_second = millis();
    static unsigned long timme_pasted_send = millis();

    if (time - time_passed_init >= 5000  && !client.isConnected())
    {
        client.wifiConnect();
        time_passed_second = millis();
    }
    else if (time - timme_pasted_send >= 2000)
    {
        
        String message = "Hello there, my friend";
        unsigned char table[22];
    
        for (size_t i = 0; i < 22; i++)
        {
            table[i] = message[i];
        }
    
        // Send the message
        client.sendMessage(22, table, EnumIPType::WATCH);
        timme_pasted_send = millis();
    }
    if (time - time_passed_second >= 1000)
    {
        Serial.println("1 second passed");
        time_passed_second = millis();
    }
}



//1- On commence par recevoir du serveur les personnes qui sont accessible
//2- Il faut envoyer qui on est apres


void FonctionTest1Communication()
{
    //Receive 
    //then send

    
}
