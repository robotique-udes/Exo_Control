#include <Arduino.h>

#include <Nextion.h>

#include "touchScreen.h"
#include "enumScreenCallbacks.h"


TouchScreen& screen = TouchScreen::getInstance();

void callback(void *ptr){
  Serial.println("Pressed EnumScreenCallback::BUTTON_ARMED");

  // On recoi le ptr vers le boutton en parametre. On peut alors l'utiliser comme ca : 
  NexButton *bARMED = static_cast<NexButton*>(ptr);

  // Change le text du bouton pour "Pressed" sur l'ecran
  bARMED->setText("Pressed");
}

void setup(void) {    
  Serial.begin(9600);

  nexInit();

  screen.setCallback(EnumScreenCallback::BUTTON_ARMED, &callback);
}

void loop() {
  screen.update();
}
