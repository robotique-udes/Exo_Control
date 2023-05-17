#include "touchScreen.h"

// Nextion TX to pin 2 and RX to pin 3 of Arduino
//---------------------------------------BOUTONS (page, id, name)--------------------------------------------
//main
NexButton Screen::bNeutral = NexButton(0, 6, "bNeutral"); //Clutch ON
NexButton Screen::bAuto = NexButton(0, 7, "bAuto"); //Clutch OFF    (Motor Control ON)

//test
NexButton Screen::bStand = NexButton(1, 6, "bStand"); //test standing straight
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

NexSlider Screen::Pslider = NexSlider(4, 6, "Pslider");   //P slider
NexNumber Screen::Pindicator = NexNumber(4, 12, "Pindic");       //P Float
NexSlider Screen::Islider = NexSlider(4, 10, "Islider");  //I Slider
NexNumber Screen::Iindicator = NexNumber(4, 13, "Iindic");       //I Float
NexSlider Screen::Dslider = NexSlider(4, 11, "Dslider");  //D Slider
NexNumber Screen::Dindicator = NexNumber(4, 14, "Dindic");       //D Float

uint32_t P;
uint32_t Ptemp;
uint32_t I;
uint32_t Itemp;
uint32_t D;
uint32_t Dtemp;

Screen::Screen(){
  Serial.print("BONJOUR");
  //Motor* Screen::motor = m;
  //motor = m;
  //          PUSH -> ACTIVE QUAND TU PRESS               POP -> ACTIVE QUAND TU RELACHE
  //bLeft_Hip_UP.attachPush(LeftHip_UP, &bLeft_Hip_UP); //Press
  //main
  bNeutral.attachPush(StateNEUTRAL, &bNeutral); //mode neutral
  bAuto.attachPush(StateAUTOMATIC, &bAuto); //mode auto
  //test
  bStand.attachPush(TestStand, &bStand); //test stand 
  bStand.attachPop(TestNeutral, &bStand);  //stopping test
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
  Islider.attachPop(Ivalue, &Islider);
  Islider.attachPush(Ivalue, &Islider);
  Dslider.attachPop(Dvalue, &Dslider);
  Dslider.attachPush(Dvalue, &Dslider);


  Pslider.setValue(50);
  Islider.setValue(50);
  Dslider.setValue(50);
}

Screen::~Screen(){

}


//---------------------------------------STATE--------------------------------------------
//STATE AUTO (1)
void Screen::StateNEUTRAL(void *ptr)
{ 
  Serial.print("\nNEUTRAL");
}

//STATE NEUTRAL (0)
void Screen::StateAUTOMATIC(void *ptr)
{
  Serial.print("\nAUTO");
}

//---------------------------------------TEST--------------------------------------------
//Left
void Screen::TestStand(void *ptr)
{
  Serial.print("\nTEST STANDING");  
}

void Screen::TestSquat(void *ptr){
  Serial.print("\nTEST SQUATTING");
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
  int iteration = 500;
  double rightTotal=0;
  double leftTotal=0;
  double height;
  for(int i=0;i<iteration;i++)
  {
    rightTotal+= motor->sonarScanR();
    leftTotal+= motor->sonarScanL();
  }
  height=((rightTotal+leftTotal)/(iteration*2));
  motor->setHeight(height);
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

void Screen::setHeight(double h)
{
  motor->setHeight(h+5);
}


//---------------------------------------PID--------------------------------------------
void Screen::PID(void *ptr){
  bSavePID.Set_background_color_bco(40689);
  Pslider.setValue(P); 
  Pindicator.setValue(P);
  Islider.setValue(I);
  Iindicator.setValue(I);
  Dslider.setValue(D);
  Dindicator.setValue(D);
}

void Screen::SavePID(void *ptr){
  P = Ptemp;
  I = Itemp;
  D = Dtemp; 
  bSavePID.Set_background_color_bco(40689);
}

//SETTING P VALUE
void Screen::Pvalue(void *ptr){
  Pslider.getValue(&Ptemp); 
  Pindicator.setValue(Ptemp);
  bSavePID.Set_background_color_bco(65520);
}

//SETTING I VALUE
void Screen::Ivalue(void *ptr){
  Islider.getValue(&Itemp);
  Iindicator.setValue(Itemp);
  bSavePID.Set_background_color_bco(65520);
}

//SETTING D VALUE
void Screen::Dvalue(void *ptr){
  Dslider.getValue(&Dtemp);
  Dindicator.setValue(Dtemp);
  bSavePID.Set_background_color_bco(65520);
}

void Screen::nextLoop(){
  nexLoop(nex_listen_list);
}

NexTouch *Screen::nex_listen_list[] = { //notif quand appele
        &bNeutral,
        &bAuto,
        &bStand,
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
        &Islider,
        &Iindicator,
        &Dslider,
        &Dindicator,
        NULL
};
