#include "touchScreen.h"


Screen::Screen()
{

  /*bMAIN_page.attachPush(MainPage, &bMAIN_page);
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
  bRight_Knee_EXTRACT.attachPop(RightKnee_NEURAL, &bRight_Knee_EXTRACT);*/
}

Screen::~Screen(){}


//---------------------------------------STATE--------------------------------------------
//STATE AUTO (1)
void Screen::StateON(void *ptr){  
  Serial.print("\nAUTO");
 mode = auto;
}

//STATE NEUTRAL (0)
void Screen::StateOFF(void *ptr){
  Serial.print("\nNEUTRE");
  mode = neutral;
}

//---------------------------------------HIPS--------------------------------------------
//Left
void Screen::LeftHip_UP(void *ptr){
  Serial.print("\nLEFT HIP UP");   
  
}

void Screen::LeftHip_DOWN(void *ptr){
  Serial.print("\nLEFT HIP DOWN");
 
}

void Screen::LeftHip_NEUTRAL(void *ptr){
  Serial.print("\nLEFT HIP NEUTRAL");
 
}

//Right
void Screen::RightHip_UP(void *ptr){
  Serial.print("\nRIGHT HIP UP"); 
   
}

void Screen::RightHip_DOWN(void *ptr){
  Serial.print("\nRIGHT HIP DOWN");
}

void Screen::RightHip_NEUTRAL(void *ptr){
  Serial.print("\nRIGHT HIP NEUTRAL");
}

//---------------------------------------KNEES--------------------------------------------
//Left
void Screen::LeftKnee_RETRACT(void *ptr){
  Serial.print("\nLEFT KNEE RETRACT");
}

void Screen::LeftKnee_EXTRACT(void *ptr){
  Serial.print("\nLEFT KNEE EXTRACT");
}

void Screen::LeftKnee_NEUTRAL(void *ptr){
  Serial.print("\nLEFT KNEE NEUTRAL");
}

//Right
void Screen::RightKnee_RETRACT(void *ptr){
  Serial.print("\nRIGHT KNEE RETRACT");
}

void Screen::RightKnee_EXTRACT(void *ptr){
  Serial.print("\nRIGHT KNEE EXTRACT");
}

void Screen::RightKnee_NEURAL(void *ptr){
  Serial.print("\nRIGHT KNEE NEUTRAL");
}

//---------------------------------------PAGES--------------------------------------------
void Screen::MainPage(void *ptr){
}

void Screen::RobotPage(void *ptr){

}

void Screen::SettingsPage(void *ptr){
}

void Screen::GoosePage(void *ptr){
}

void Screen::nextLoop(){
nexLoop(nex_listen_list);
}

