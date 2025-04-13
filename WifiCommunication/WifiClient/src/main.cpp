#include "WifiClient.h"
#include "MessageBuilder/MessageBuilder.h"

WifiClient* wificlient;

void setup() {
    // Start the Serial communication
    Serial.begin(115200);    
    delay(5000);

    // Initialize the WifiClient
    wificlient = WifiClient::GetInstance();
}

void loop() {
    static unsigned long time_passed_init = millis();
    static unsigned long time_passed_second = millis();
    static unsigned long time_pasted_send = millis();
    unsigned long time = millis();

    // call the update function
    wificlient->upDate();


    if (time - time_pasted_send >= 2000)
    {
        
        MessageBuilder message = MessageBuilder();
        message.add("Helloooo, it's me");
        message.buildMessage();
    
        // Send the message
        wificlient->sendMessage(message.getMessageLength(), message.getMessage(), EnumIPType::WATCH);
        time_pasted_send = millis();
    }
    if (time - time_passed_second >= 1000)
    {
        Serial.println("1 second passed");
        time_passed_second = time;
    }
        
}



//1- On commence par recevoir du serveur les personnes qui sont accessible
//2- Il faut envoyer qui on est apres


void FonctionTest1Communication()
{
    //Receive 
    //then send

    
}
