#include <Arduino.h>
#include <HardwareSerial.h>
#include <string>
#include "motorControl.h"



HardwareSerial SerialPort(1);

char bitOfMessage = ' ';
std::string message = " ";

int MGGin1Val = 0;
int MGGin2Val = 0;
int MGGpwmMotor = 0;
int MGDin1Val = 0;
int MGDin2Val = 0;
int MGDpwmMotor = 0;
int MHGin1Val = 0;
int MHGin2Val = 0;
int MHGpwmMotor = 0;
int MHDin1Val = 0;
int MHDin2Val = 0;
int MHDpwmMotor = 0;

Motor motor;



void setup()
{
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 18, 19);
  pinMode(D1_IN1_A, OUTPUT);
  pinMode(D1_IN2_A, OUTPUT);
  pinMode(D1_EN_A, OUTPUT);
  pinMode(D2_IN1_A, OUTPUT);
  pinMode(D2_IN1_A, OUTPUT);
  pinMode(D2_IN1_A, OUTPUT);
  Serial.println("setup done");

}



void loop()
{
  while (SerialPort.available())
  {
    bitOfMessage = char(SerialPort.read());
    if (bitOfMessage != '\n'){
      message += bitOfMessage;
    }
    else{
      MGGin1Val = std::stoi(message.substr(2,1));
      MGGin2Val = std::stoi(message.substr(3,1));;
      MGGpwmMotor = std::stoi(message.substr(4,3));
      MGDin1Val = std::stoi(message.substr(8,1));;
      MGDin2Val = std::stoi(message.substr(9,1));;
      MGDpwmMotor = std::stoi(message.substr(10,3));
      MHGin1Val = std::stoi(message.substr(14,1));;
      MHGin2Val = std::stoi(message.substr(15,1));;
      MHGpwmMotor = std::stoi(message.substr(16,3));
      MHDin1Val = std::stoi(message.substr(20,1));;
      MHDin2Val = std::stoi(message.substr(21,1));;
      MHDpwmMotor = std::stoi(message.substr(22,3));

      motor.motorSetSpeed(MOTEUR_GENOU_GAUCHE, MGGin1Val, MGGin2Val, MGGpwmMotor);
      motor.motorSetSpeed(MOTEUR_GENOU_DROIT, MGDin1Val, MGDin2Val, MGDpwmMotor);
      motor.motorSetSpeed(MOTEUR_HANCHE_GAUCHE, MHGin1Val, MHGin2Val, MHGpwmMotor);
      motor.motorSetSpeed(MOTEUR_HANCHE_DROITE, MHDin1Val, MHDin2Val, MHDpwmMotor);

      Serial.print(MGGin1Val);
      Serial.print(MGGin2Val);
      Serial.print(MGGpwmMotor);
      Serial.print("   ");
      Serial.print(MGDin1Val);
      Serial.print(MGDin2Val);
      Serial.print(MGDpwmMotor);
      Serial.print("   ");
      Serial.print(MHGin1Val);
      Serial.print(MHGin2Val);
      Serial.print(MHGpwmMotor);
      Serial.print("   ");
      Serial.print(MHDin1Val);
      Serial.print(MHDin2Val);
      Serial.println(MHDpwmMotor);
    
    }
  }
}
