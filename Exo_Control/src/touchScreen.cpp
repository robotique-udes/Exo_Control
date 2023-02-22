#include "touchScreen.h"

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino


//---------------------------------------BOUTONS (page, id, name)--------------------------------------------
NexButton bON = NexButton(0, 4, "b0"); // STATE ON
NexButton bOFF = NexButton(0, 5, "b1"); // STATE OFF

NexButton bLeft_Hip_UP = NexButton(1, 14, "b9"); // Left Hip UP
NexButton bLeft_Hip_DOWN = NexButton(1, 13, "b8"); // Left Hip Down
NexButton bRight_Hip_UP = NexButton(1, 16, "b11"); // Right Hip UP
NexButton bRight_Hip_DOWN = NexButton(1, 15, "b10"); // Right Hip Down

NexButton bLeft_Knee_RETRACT = NexButton(1, 9, "b0"); // Left Hip UP
NexButton bLeft_Knee_EXTRACT = NexButton(1, 10, "b1"); // Left Hip Down
NexButton bRight_Knee_RETRACT = NexButton(1, 12, "b7"); // Right Hip UP
NexButton bRight_Knee_EXTRACT = NexButton(1, 11, "b6"); // Right Hip Down

NexButton bMAIN_page = NexButton(0, 6, "b2") = NexButton(1, 2, "b2") = NexButton(2, 2, "b2") = NexButton(3, 2, "b2"); //Page 0
NexButton bROBOT_page = NexButton(0, 7, "b3") = NexButton(1, 3, "b3") = NexButton(2, 3, "b3") = NexButton(3, 3, "b3"); //Page 1
NexButton bSETTINGS_page = NexButton(0, 8, "b4") = NexButton(1, 4, "b4") = NexButton(2, 4, "b4") = NexButton(3, 4, "b4"); //Page 2
NexButton bGOOSE_page = NexButton(0, 9, "b5") = NexButton(1, 5, "b5") = NexButton(2, 5, "b5") = NexButton(3, 5, "b5"); //Page 3

//---------------------------------------TEXTES--------------------------------------------
NexText t0 = NexText(0, 3, "t0"); //TEXTE STATE




//---------------------------------------NOTIFS--------------------------------------------
NexTouch *nex_listen_list[] = { //notif quand appele

&bMAIN_page, //page 0
&bROBOT_page, //page 1
&bSETTINGS_page, //page 2
&bGOOSE_page, //page 3

&bON, //stateON (Auto)
&bOFF, //stateOFF (Neutra,)

&bLeft_Hip_UP, //HIPS
&bLeft_Hip_DOWN,
&bRight_Hip_UP,
&bRight_Hip_DOWN,

&bLeft_Knee_RETRACT, //KNESS
&bLeft_Knee_EXTRACT,
&bRight_Knee_RETRACT,
&bRight_Knee_EXTRACT,
NULL
};



//---------------------------------------STATE--------------------------------------------
//STATE AUTO (1)
void StateON(void *ptr){  
  Serial.print("\nAUTO");
 mode = auto;
}

//STATE NEUTRAL (0)
void StateOFF(void *ptr){
  Serial.print("\nNEUTRE");
  mode = neutral;
}

//---------------------------------------HIPS--------------------------------------------
//Left
void LeftHip_UP(void *ptr){
  Serial.print("\nLEFT HIP UP");   
  
}

void LeftHip_DOWN(void *ptr){
  Serial.print("\nLEFT HIP DOWN");
 
}

void LeftHip_NEUTRAL(void *ptr){
  Serial.print("\nLEFT HIP NEUTRAL");
 
}

//Right
void RightHip_UP(void *ptr){
  Serial.print("\nRIGHT HIP UP"); 
   
}

void RightHip_DOWN(void *ptr){
  Serial.print("\nRIGHT HIP DOWN");
}

void RightHip_NEUTRAL(void *ptr){
  Serial.print("\nRIGHT HIP NEUTRAL");
}

//---------------------------------------KNEES--------------------------------------------
//Left
void LeftKnee_RETRACT(void *ptr){
  Serial.print("\nLEFT KNEE RETRACT");
}

void LeftKnee_EXTRACT(void *ptr){
  Serial.print("\nLEFT KNEE EXTRACT");
}

void LeftKnee_NEUTRAL(void *ptr){
  Serial.print("\nLEFT KNEE NEUTRAL");
}

//Right
void RightKnee_RETRACT(void *ptr){
  Serial.print("\nRIGHT KNEE RETRACT");
}

void RightKnee_EXTRACT(void *ptr){
  Serial.print("\nRIGHT KNEE EXTRACT");
}

void RightKnee_NEURAL(void *ptr){
  Serial.print("\nRIGHT KNEE NEUTRAL");
}

//---------------------------------------PAGES--------------------------------------------
void MainPage(void *ptr){
}

void RobotPage(void *ptr){

}

void SettingsPage(void *ptr){
}

void GoosePage(void *ptr){
}


//---------------------------------------SETUP--------------------------------------------
void setup() {

  Serial.begin(9600);
  nexInit();  

  bMAIN_page.attachPush(MainPage, &bMAIN_page);
  bROBOT_page.attachPush(RobotPage, &bROBOT_page);

  bON.attachPush(StateON, &bON);
  bOFF.attachPush(StateOFF, &bOFF);
  

  //          PUSH -> ACTIVE QUAND TU PRESS               POP -> ACTIVE QUAND TU RELACHE
  bLeft_Hip_UP.attachPush(LeftHip_UP, &bLeft_Hip_UP); //Push
  bLeft_Hip_UP.attachPop(LeftHip_NEUTRAL, &bLeft_Hip_UP); 
  bLeft_Hip_DOWN.attachPush(LeftHip_DOWN, &bLeft_Hip_DOWN); //Push
  bLeft_Hip_DOWN.attachPop(LeftHip_NEUTRAL, &bLeft_Hip_DOWN);

  bRight_Hip_UP.attachPush(RightHip_UP, &bRight_Hip_UP); //Push
  bRight_Hip_UP.attachPop(RightHip_NEUTRAL, &bRight_Hip_UP);
  bRight_Hip_DOWN.attachPush(RightHip_DOWN, &bRight_Hip_DOWN); //Push
  bRight_Hip_DOWN.attachPop(RightHip_NEUTRAL, &bRight_Hip_DOWN);

  bLeft_Knee_RETRACT.attachPush(LeftKnee_RETRACT, &bLeft_Knee_RETRACT); //Push
  bLeft_Knee_RETRACT.attachPop(LeftKnee_NEUTRAL, &bLeft_Knee_RETRACT);
  bLeft_Knee_EXTRACT.attachPush(LeftKnee_EXTRACT, &bLeft_Knee_EXTRACT); //Push
  bLeft_Knee_EXTRACT.attachPop(LeftKnee_NEUTRAL, &bLeft_Knee_EXTRACT); 

  bRight_Knee_RETRACT.attachPush(RightKnee_RETRACT, &bRight_Knee_RETRACT); //Push
  bRight_Knee_RETRACT.attachPop(RightKnee_NEURAL, &bRight_Knee_RETRACT);
  bRight_Knee_EXTRACT.attachPush(RightKnee_EXTRACT, &bRight_Knee_EXTRACT); //Push
  bRight_Knee_EXTRACT.attachPop(RightKnee_NEURAL, &bRight_Knee_EXTRACT);
}

//---------------------------------------LOOP--------------------------------------------
void loop() {

}

