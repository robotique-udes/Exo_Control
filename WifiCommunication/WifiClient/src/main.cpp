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
    static unsigned long time_passed_second = millis();
    unsigned long time = millis();

    wificlient->upDate();

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
