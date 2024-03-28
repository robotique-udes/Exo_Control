#include <Arduino.h>

#include "touchScreen.h"
#include "enumScreenCallbacks.h"


TouchScreen& screen = TouchScreen::getInstance();

void callback(void *ptr){
  Serial.println("Pressed EnumScreenCallback::BUTTON_ARMED");
}

void setup(void) {    
  Serial.begin(9600);

  nexInit();

  screen.setCallback(EnumScreenCallback::BUTTON_ARMED, &callback);
}

void loop() {
  screen.update();
}
