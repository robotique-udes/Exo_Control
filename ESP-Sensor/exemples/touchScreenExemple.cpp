#include <Arduino.h>

#include <Nextion.h>
#include "touchScreen.h"
#include "callbackSetup.h"

// Important : Le tx RX est 16 et 17 repectivement !!


TouchScreen& screen = TouchScreen::getInstance();

void setup(void) {    
  Serial.begin(9600);

  nexInit();
  setupCallbacks();
}

void loop() {
  screen.update();
  screen.print("asdf\0");
}
