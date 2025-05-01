#include "HMI.h"

HMI * watch_interface;

void setup() {
  Serial.begin(115200);
  delay(3000);

  watch.begin();
  watch_interface = new HMI();

  WifiServer::GetInstance()->Initialise();

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

  WifiServer::upDate();

  // update the watch interface
  watch_interface->update();

}
