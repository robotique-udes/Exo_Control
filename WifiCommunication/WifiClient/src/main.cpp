#include "WifiClient.h"
#include "MessageBuilder/MessageBuilder.h"

WifiClient client;

void setup() {
    // Start the Serial communication
    Serial.begin(115200);    
    delay(5000);
    client.wifiConnect();
}

void loop() {
    // Connect the client after 5 seconds
    static unsigned long time_passed_init = millis();
    static unsigned long time_passed_second = millis();
    static unsigned long time_pasted_send = millis();
    unsigned long time = millis();


    if (time - time_pasted_send >= 2000)
    {
        
        MessageBuilder message = MessageBuilder();
        message.add("Helloooo, it's me");
        message.buildMessage();
    
        // Send the message
        client.sendMessage(22, message.getMessage(), EnumIPType::WATCH);
        time_pasted_send = millis();
    }
    if (time - time_passed_second >= 1000)
    {
        Serial.println("1 second passed");
        time_passed_second = millis();
        
        int longueur_ips = 0;

        longueur_ips = client.dataAvailable();
        if(longueur_ips > 0)
        {
            Serial.print("Message Received: ");
            Serial.println(longueur_ips); 
            unsigned char IPs[longueur_ips];
            client.receiveMessage(IPs);
            client.deserializeMessage(IPs, longueur_ips);  
        }
        
    }
}



//1- On commence par recevoir du serveur les personnes qui sont accessible
//2- Il faut envoyer qui on est apres


void FonctionTest1Communication()
{
    //Receive 
    //then send

    
}
