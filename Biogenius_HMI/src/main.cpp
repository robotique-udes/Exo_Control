#include <Arduino.h>
// #include <Servo.h>
// #include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h" //Zoboomafou was here
#include <HardwareSerial.h>
//bruh



// Screen ecran;
Motor motor;
Test test01;
<<<<<<< Updated upstream
=======
Imu imu01;
using namespace std;

HardwareSerial SerialPort(2);
>>>>>>> Stashed changes

//---------------------------------------BOUTONS (page, id, name)--------------------------------------------
NexButton bON = NexButton(0, 4, "b0"); // STATE ON
NexButton bOFF = NexButton(0, 5, "b1"); // STATE OFF

//---------------------------------------NOTIFS--------------------------------------------
NexTouch *nex_listen_list[] = { //notif quand appele
&bON, //stateON (Auto)
&bOFF, //stateOFF (Neutra,)
NULL
};

//---------------------------------------STATE--------------------------------------------
//STATE AUTO (1)
void StateON(void *ptr){  
  Serial.print("\nAUTO");
}

//STATE NEUTRAL (0)
void StateOFF(void *ptr){
  Serial.print("\nNEUTRE");
}

//---------------------------------------SETUP--------------------------------------------
void setup() {
  nexInit();
  motor.setPins();
<<<<<<< Updated upstream


  Serial.println("Ini motor exo----------");
}

void loop()
{
  motor.testMotor();
  motor.testRelais();
=======
  
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  bON.attachPush(StateON, &bON);
  bOFF.attachPush(StateOFF, &bOFF);
  
  //          PUSH -> ACTIVE QUAND TU PRESS               POP -> ACTIVE QUAND TU RELACHE
  //bLeft_Hip_UP.attachPush(LeftHip_UP, &bLeft_Hip_UP); //Press

}

>>>>>>> Stashed changes

//---------------------------------------LOOP--------------------------------------------
void loop() {

  nexLoop(nex_listen_list);
}



