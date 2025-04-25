#include "HMI.h"

void setup() {
    Serial.begin(115200);
    // Connect to Wifi network.
    delay(3000);
  
    watch.begin();
    beginLvglHelper();

    HMI watch_interface = HMI();
    watch_interface.setup();
  
  }
  
  void loop() 
  {
    static unsigned long previousMillis = 0; // Stores the last time a message was printed
    unsigned long currentMillis = millis();
    static int compteur = 0;

    if (currentMillis - previousMillis >= 1000) 
    {
        previousMillis = currentMillis;
        compteur++;
        Serial.print(compteur);
        Serial.println("   1 second has pass");
    }
    lv_task_handler();

  }
