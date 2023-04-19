#include <Arduino.h>
#include <Nextion.h>
#include <string.h>
#include <HardwareSerial.h>
using namespace std;

HardwareSerial SerialPort(2);

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
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  bON.attachPush(StateON, &bON);
  bOFF.attachPush(StateOFF, &bOFF);
  
  //          PUSH -> ACTIVE QUAND TU PRESS               POP -> ACTIVE QUAND TU RELACHE
  //bLeft_Hip_UP.attachPush(LeftHip_UP, &bLeft_Hip_UP); //Press

}


//---------------------------------------LOOP--------------------------------------------
void loop() {

  nexLoop(nex_listen_list);
}

