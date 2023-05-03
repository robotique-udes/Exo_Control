#include "touchScreen.h"

Screen::Screen(){}

Screen::~Screen(){}



//---------------------------------------STATE--------------------------------------------
//STATE AUTO (1)
void Screen::StateON(void *ptr)
{ 
  Serial.print("\nAUTO");
  mode = autobbb;
}

//STATE NEUTRAL (0)
void Screen::StateOFF(void *ptr)
{
  Serial.print("\nNEUTRE");
  mode = neutral;
}

//---------------------------------------HIPS--------------------------------------------
//Left
void Screen::LeftHip_UP(void *ptr)
{
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

