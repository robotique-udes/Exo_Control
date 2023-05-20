#include "touchScreen.h"

// Nextion TX to pin 2 and RX to pin 3 of Arduino
//---------------------------------------BOUTONS (page, id, name)--------------------------------------------
//main
NexButton Screen::bDISARMED = NexButton(0, 6, "bDISARMED"); //DISARMED
NexButton Screen::bARMED = NexButton(0, 8, "bARMED"); //Clutch ON   ( ARMED)
NexButton Screen::bMOTORISED = NexButton(0, 7, "bMOTORISED");// CLUTCH ON + MOTOR ON 

//test
NexButton Screen::bWalk = NexButton(1, 6, "bStand"); //test standing straight
NexButton Screen::bSquat = NexButton(1, 7, "bSquat"); //test squat

//calibration
NexButton Screen::bAutoCalib = NexButton(2, 5, "bAutoCalib"); //Auto calibration
NexButton Screen::b4f = NexButton(2, 7, "b4f"); //Calibration at (4')
NexButton Screen::b5f8 = NexButton(2, 8, "b5f8"); //Calibration at (5'8")
NexButton Screen::b5f10 = NexButton(2, 6, "b5f10"); //Calibration at (5'10")
NexButton Screen::b6f = NexButton(2, 9, "b6f"); //Calibration at (6')

//PID
NexButton Screen::bPID0 = NexButton(0, 4, "bPID");
NexButton Screen::bPID1 = NexButton(1, 4, "bPID");
NexButton Screen::bPID2 = NexButton(2, 4, "bPID");
NexButton Screen::bSavePID = NexButton(4, 4, "bSavePID");

NexSlider Screen::Pslider = NexSlider(4, 6, "Pslider");   //Power motor slider
NexNumber Screen::Pindicator = NexNumber(4, 12, "Pindic");//Power motor indicator

uint32_t P= 175;
uint32_t Ptemp;

Screen::Screen(){
  Serial.print("BONJOUR");
  //Motor* Screen::motor = m;
  //motor = m;
  //          PUSH -> ACTIVE QUAND TU PRESS               POP -> ACTIVE QUAND TU RELACHE
  //bLeft_Hip_UP.attachPush(LeftHip_UP, &bLeft_Hip_UP); //Press
  //main
  bDISARMED.attachPush(Disarmed, &bDISARMED);
  bARMED.attachPush(Armed, &bARMED);
  bMOTORISED.attachPush(Motorised, &bMOTORISED);
  //test
  bWalk.attachPush(TestWalk, &bWalk); //test stand 
  bWalk.attachPop(TestNeutral, &bWalk);  //stopping test
  bSquat.attachPush(TestSquat, &bSquat); //test squat
  bSquat.attachPop(TestNeutral, &bSquat);  //stopping test 
  //calibration
  bAutoCalib.attachPush(AutoCalibration, &bAutoCalib);
  b4f.attachPush(Quack, &b4f);
  b5f8.attachPush(Calib_5f8, &b5f8);
  b5f10.attachPush(Calib_5f10, &b5f10);
  b6f.attachPush(Calib_6f, &b6f);
  
  bPID0.attachPush(PID, &bPID0);
  bPID1.attachPush(PID, &bPID1);
  bPID2.attachPush(PID, &bPID2);
  bSavePID.attachPush(SavePID, &bSavePID);

  Pslider.attachPop(Pvalue, &Pslider);
  Pslider.attachPush(Pvalue, &Pslider);


  Pslider.setValue(50);
}

Screen::~Screen(){

}


//---------------------------------------STATE--------------------------------------------
//DISARMED
void Screen::Disarmed(void *ptr)
{ 
  Serial.print("\nDisarmed");
  motor->setAllRelais(OFF);
  motor->setMotorMode(OFF);
}

//ARMED
void Screen::Armed(void *ptr)
{
  Serial.print("\nArmed");
  motor->setAllRelais(ON);
  motor->setMotorMode(OFF);
}

//MOTORISED
void Screen::Motorised(void *ptr)
{
  Serial.print("\nMotorised");
  motor->setAllRelais(ON);
  motor->setMotorMode(ON);
}

//---------------------------------------TEST--------------------------------------------
//Left
void Screen::TestWalk(void *ptr) //TEST WALKING
{
  Serial.print("\nTEST WALKING"); 
  motor->setSonarState(OFF);
}

void Screen::TestSquat(void *ptr){//TEST SQUATTING
  Serial.print("\nTEST SQUATTING");
  motor->setSonarState(ON);
}

void Screen::TestNeutral(void *ptr){
  Serial.print("\nTEST IN NEUTRAL");
}


//---------------------------------------CALIBRATION--------------------------------------------
//Left
void Screen::AutoCalibration(void *ptr){
  Serial.print("\nCalibrating...");
  bAutoCalib.Set_background_color_bco(40689);

  bAutoCalib.setText("Calibrating...");
  //Calibration distance mesured = distance of calibration
  int iteration = 50;
  double rightTotal=0;
  double leftTotal=0;
  double height;
  for(int i=0;i<iteration;i++)
  {
    Serial.println(i);
    rightTotal+= motor->sonarScanR();
    leftTotal+= motor->sonarScanL();
  }
  height=((rightTotal+leftTotal)/(iteration*2));
  motor->setHeight(height+10);
  Serial.print("\nCalibrated!");
  bAutoCalib.setText("CALIBRATED!");
}

void Screen::Quack(void *ptr){
  Serial.print("\nQUACK");
}

void Screen::Calib_5f8(void *ptr){
  Serial.print("\nCALIBRATED AT 5' 8\"");
  motor->setHeight(40);
}

//Right
void Screen::Calib_5f10(void *ptr){
  Serial.print("\nCALIBRATED AT 5' 10\"");
  motor->setHeight(45);
}

void Screen::Calib_6f(void *ptr){
  Serial.print("\nCALIBRATED AT 6'");
  motor->setHeight(50);
}


//---------------------------------------PID--------------------------------------------
void Screen::PID(void *ptr){
  bSavePID.Set_background_color_bco(40689);
  Pslider.setValue(P); 
  Serial.print(Pslider.getValue(&Ptemp));
}

void Screen::SavePID(void *ptr){
  P = Ptemp;
  bSavePID.Set_background_color_bco(40689);
  motor->setPower(P);
}

//SETTING P VALUE
void Screen::Pvalue(void *ptr){
  Pslider.getValue(&Ptemp); 
  Pindicator.setValue(Ptemp);
  bSavePID.Set_background_color_bco(65520);
}


void Screen::nextLoop(){
  nexLoop(nex_listen_list);
}

NexTouch *Screen::nex_listen_list[] = { //notif quand appele
        &bDISARMED,
        &bARMED,
        &bMOTORISED,
        &bWalk,
        &bSquat,
        &bAutoCalib,
        &b4f,
        &b5f8,
        &b5f10,
        &b6f,
        &bPID0,
        &bPID1,
        &bPID2,
        &bSavePID,
        &Pslider,
        &Pindicator,
        NULL
};
