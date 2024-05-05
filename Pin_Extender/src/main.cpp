#include <PCA9505_9506.h>

/*
 *  Output toggling for the PCA9505/06 I/O expander
 */

PCA9505_06 PinExtender;

void setup() {
  PinExtender.begin();   //  Default address 0x20
  PinExtender.pinMode(7, OUTPUT); // IO0_7
  PinExtender.pinMode(8, OUTPUT); // IO1_0
  PinExtender.pinMode(9, OUTPUT); // IO1_1

}

void loop() {
  delay(1000);
  
  PinExtender.digitalWrite(7, PinExtender.digitalRead(7) ? LOW : HIGH);
  PinExtender.digitalWrite(8, PinExtender.digitalRead(8) ? LOW : HIGH);
  PinExtender.digitalWrite(9, PinExtender.digitalRead(9) ? LOW : HIGH);
}
