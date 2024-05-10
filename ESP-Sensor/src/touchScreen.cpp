#include "touchScreen.h"

TouchScreen *TouchScreen::instance;

TouchScreen::TouchScreen(){

    bDISARMED = new NexButton(0, 6, "bDISARMED");
    bARMED = new NexButton(0, 8, "bARMED"); //Clutch ON   ( ARMED)
    bMOTORISED = new NexButton(0, 7, "bMOTORISED");// CLUTCH ON + MOTOR ON 
    bWalk = new NexButton(1, 6, "bStand"); //test standing straight
    bSquat = new NexButton(1, 7, "bSquat"); //test squat
    bAutoCalib = new NexButton(2, 5, "bAutoCalib"); //Auto calibration
    b4f = new NexButton(2, 7, "b4f"); //Calibration at (4')
    b5f8 = new NexButton(2, 8, "b5f8"); //Calibration at (5'8")
    b5f10 = new NexButton(2, 6, "b5f10"); //Calibration at (5'10")
    b6f = new NexButton(2, 9, "b6f"); //Calibration at (6')
    bPID0 = new NexButton(0, 4, "bPID");
    bPID1 = new NexButton(1, 4, "bPID");
    bPID2 = new NexButton(2, 4, "bPID");
    bSavePID = new NexButton(4, 4, "bSavePID");
    Pslider = new NexSlider(4, 6, "Pslider");   //Power motor slider
    Pindicator = new NexNumber(4, 12, "Pindic");//Power motor indicator
    
    listenList[0] = bDISARMED;
    listenList[1] = bARMED;
    listenList[2] = bMOTORISED;
    listenList[3] = bWalk;
    listenList[4] = bSquat;
    listenList[5] = bAutoCalib;
    listenList[6] = b4f;
    listenList[7] = b5f8;
    listenList[8] = b5f10;
    listenList[9] = b6f;
    listenList[10] = bPID0;
    listenList[11] = bPID1;
    listenList[12] = bPID2;
    listenList[13] = bSavePID;
    listenList[14] = Pslider;
    listenList[15] = Pindicator;
    listenList[16] = NULL;

    Pslider->setValue(50);
}

void TouchScreen::update(){
    nexLoop(listenList);
}

void TouchScreen::setCallback(EnumScreenCallback callback, void (*callbackFunction)(void *ptr)){

    switch (callback)
    {
        case EnumScreenCallback::BUTTON_4F:
            b4f->attachPush(callbackFunction, b4f);
        break;
        case EnumScreenCallback::BUTTON_5F10:
            b5f10->attachPush(callbackFunction, b5f10);
        break;
        case EnumScreenCallback::BUTTON_5F8:
            b5f8->attachPush(callbackFunction, b5f8);
        break;
        case EnumScreenCallback::BUTTON_6F:
            b6f->attachPush(callbackFunction, b6f);
        break;
        case EnumScreenCallback::BUTTON_ARMED:
            bARMED->attachPush(callbackFunction, bARMED);
        break;
        case EnumScreenCallback::BUTTON_AUTOCALIB :
            bAutoCalib->attachPush(callbackFunction, bAutoCalib);
        break;
        case EnumScreenCallback::BUTTON_DISARMED :
            bDISARMED->attachPush(callbackFunction, bDISARMED);
        break;
        case EnumScreenCallback::BUTTON_MOTORISED :
            bMOTORISED->attachPush(callbackFunction, bMOTORISED);
        break;
        case EnumScreenCallback::BUTTON_PID0 :
            bPID0->attachPush(callbackFunction, bPID0);
        break;
        case EnumScreenCallback::BUTTON_PID1 :
            bPID1->attachPush(callbackFunction, bPID1);
        break;
        case EnumScreenCallback::BUTTON_PID2 :
            bPID2->attachPush(callbackFunction, bPID2);
        break;
        case EnumScreenCallback::BUTTON_SAVE_PID :
            bSavePID->attachPush(callbackFunction, bSavePID);
        break;
        case EnumScreenCallback::BUTTON_SQUAT :
            bSquat->attachPush(callbackFunction, bSquat);
        break;
        case EnumScreenCallback::BUTTON_WALK :
            bWalk->attachPush(callbackFunction, bWalk);
        break;
        case EnumScreenCallback::P_INDICATOR :
            Pindicator->attachPush(callbackFunction, Pindicator);
        break;
        case EnumScreenCallback::P_SLIDER :
            Pslider->attachPush(callbackFunction, Pslider);
        break;
        default:break;
    }
}

NexNumber* TouchScreen::getIndicator(){
    return Pindicator;
}

TouchScreen& TouchScreen::getInstance(){
    if(instance == NULL){
        instance = new TouchScreen();
    }
    return *instance;
}

