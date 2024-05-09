// #include <motorControl.h>
// #include <PinExtender.h>

// /*
//  *  Output toggling for the PCA9505/06 I/O expander
//  */
// Motor motor;

// void setup()
// {
//   Serial.begin(115200);
//   PinExtender.begin(); //  Default address 0x20
//   motor.begin();
//   PinExtender.pinMode(8, OUTPUT); // IO0_8
// }

// void loop()
// {
//   delay(1000);

//   PinExtender.digitalWrite(8, PinExtender.digitalRead(8) ? LOW : HIGH);
//   motor.motorTestExtender();
//   Serial.println("Motor test main");

// }


// PCA9685-Arduino Simple Example
/*
#include "PCA9685.h"

PCA9685 pwmController;                  // Library using default B000000 (A5-A0) i2c address, and default Wire @400kHz

void setup() {
    Serial.begin(115200);               // Begin Serial and Wire interfaces
    Wire.begin();

    pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line

    pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

    pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)

    pwmController.setChannelPWM(0, 128 << 4); // Set PWM to 128/255, shifted into 4096-land

    Serial.println(pwmController.getChannelPWM(0)); // Should output 2048, which is 128 << 4
}

void loop() {
  delay(1000);
    pwmController.setChannelPWM(0, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(1, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(2, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(3, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(4, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(5, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(6, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(7, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(8, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(9, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(10, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(11, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(12, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(13, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(14, 128 << 4); // Set PWM to 128/255, shifted into 4096-land
    pwmController.setChannelPWM(15, 128 << 4); // Set PWM to 128/255, shifted into 4096-land

    Serial.print("Channel 0 PWM: ");
    Serial.println(pwmController.getChannelPWM(0));
    Serial.print("Channel 1 PWM: ");
    Serial.println(pwmController.getChannelPWM(1));
    Serial.print("Channel 2 PWM: ");
    Serial.println(pwmController.getChannelPWM(2));
    Serial.print("Channel 3 PWM: ");
    Serial.println(pwmController.getChannelPWM(3));
    Serial.print("Channel 4 PWM: ");
    Serial.println(pwmController.getChannelPWM(4));
    Serial.print("Channel 5 PWM: ");
    Serial.println(pwmController.getChannelPWM(5));
    Serial.print("Channel 6 PWM: ");
    Serial.println(pwmController.getChannelPWM(6));
    Serial.print("Channel 7 PWM: ");
    Serial.println(pwmController.getChannelPWM(7));
    Serial.print("Channel 8 PWM: ");
    Serial.println(pwmController.getChannelPWM(8));
    Serial.print("Channel 9 PWM: ");
    Serial.println(pwmController.getChannelPWM(9));
    Serial.print("Channel 10 PWM: ");
    Serial.println(pwmController.getChannelPWM(10));
    Serial.print("Channel 11 PWM: ");
    Serial.println(pwmController.getChannelPWM(11));
    Serial.print("Channel 12 PWM: ");
    Serial.println(pwmController.getChannelPWM(12));
    Serial.print("Channel 13 PWM: ");
    Serial.println(pwmController.getChannelPWM(13));
    Serial.print("Channel 14 PWM: ");
    Serial.println(pwmController.getChannelPWM(14));
    Serial.print("Channel 15 PWM: ");
    Serial.println(pwmController.getChannelPWM(15));
    Serial.println();

}*/

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Arduino.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 255; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}