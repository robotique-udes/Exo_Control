#include "touchScreen.h"

TouchScreen *TouchScreen::instance;

TouchScreen::TouchScreen(){

    motorToggle = new NexDSButton(0, 7, "motorToggle");
    clutchToggle = new NexDSButton(0, 8, "clutchToggle");
    init = new NexButton(0, 6, "init");

    resetEncoder = new NexButton(1, 2, "resetEncoder");
    autoCalibProxim = new NexButton(1, 3, "autoCalib");
    powerSlider = new NexSlider(1, 6, "powerSlider");

    angleSource = new NexDSButton(2, 2, "angleSource");
    proximToggle = new NexDSButton(2, 3, "proximToggle");
    testExo = new NexButton(2, 4, "testExo");

    button1 = new NexButton(3, 2, "button1");
    button2 = new NexButton(3, 3, "button2");
    toggle1 = new NexDSButton(3, 4, "toggle1");
    toggle2 = new NexDSButton(3, 5, "toggle2");
    slider1 = new NexSlider(3, 8, "slider1");
    
    currentString = "";
    logLines = 0;

    logText = new NexText(5, 6, "logText");
    logText->setText("");

    //Go quack : 4, 1

    listenList[0] = motorToggle;
    listenList[1] = clutchToggle;
    listenList[2] = init;
    listenList[3] = resetEncoder;
    listenList[4] = autoCalibProxim;
    listenList[5] = powerSlider;
    listenList[6] = angleSource;
    listenList[7] = proximToggle;
    listenList[8] = testExo;
    listenList[9] = button1;
    listenList[10] = button2;
    listenList[11] = toggle1;
    listenList[12] = toggle2;
    listenList[13] = slider1;
    listenList[14] = NULL;

    powerSlider->setValue(50);
}

void TouchScreen::update(){
    nexLoop(listenList);
}

void TouchScreen::setCallback(EnumScreenCallback callback, void (*callbackFunction)(void *ptr)){

    switch (callback)
    {
        case EnumScreenCallback::BUTTON_TOGGLE_MOTOR:
            motorToggle->attachPush(callbackFunction, motorToggle);
        break;
        case EnumScreenCallback::BUTTON_TOGGLE_CLUTCH:
            clutchToggle->attachPush(callbackFunction, clutchToggle);
        break;
        case EnumScreenCallback::BUTTON_INIT:
            init->attachPush(callbackFunction, init);
        break;
        case EnumScreenCallback::BUTTON_RESET_ENCODER:
            resetEncoder->attachPush(callbackFunction, resetEncoder);
        break;
        case EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM:
            autoCalibProxim->attachPush(callbackFunction, autoCalibProxim);
        break;
        case EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE :
            angleSource->attachPush(callbackFunction, angleSource);
        break;
        case EnumScreenCallback::BUTTON_TOGGLE_PROXIM :
            proximToggle->attachPush(callbackFunction, proximToggle);
        break;
        case EnumScreenCallback::BUTTON_TEST :
            testExo->attachPush(callbackFunction, testExo);
        break;
        case EnumScreenCallback::BUTTON_DEV_1 :
            button1->attachPush(callbackFunction, button1);
        break;
        case EnumScreenCallback::BUTTON_DEV_2 :
            button2->attachPush(callbackFunction, button2);
        break;
        case EnumScreenCallback::BUTTON_DEV_TOGGLE_1 :
            toggle1->attachPush(callbackFunction, toggle1);
        break;
        case EnumScreenCallback::BUTTON_DEV_TOGGLE_2 :
            toggle2->attachPush(callbackFunction, toggle2);
        break;
        case EnumScreenCallback::SLIDER_DEV_1 :
            slider1->attachPush(callbackFunction, slider1);
        break;
        case EnumScreenCallback::SLIDER_MOTOR_POWER :
            powerSlider->attachPop(callbackFunction, powerSlider);
        break;
        default:break;
    }
}

void TouchScreen::println(string toPrint){
    currentString = currentString + toPrint + "\r\n";
    
    if(logLines > 11){
        currentString = currentString.substr(currentString.find("\r\n", 0) + 2, currentString.length());
    }
    
    logText->setText(currentString.c_str());

    logLines++;
}

TouchScreen& TouchScreen::getInstance(){
    if(instance == NULL){
        instance = new TouchScreen();
    }
    return *instance;
}

