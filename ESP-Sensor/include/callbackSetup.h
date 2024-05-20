#ifndef CALLBACKSETUP_H
#define CALLBACKSETUP_H


#include <Arduino.h>
#include <Nextion.h>
#include "exoSettings.h"
#include "enumExoStates.h"
#include "touchScreen.h"

void callbackButtonDisarmed(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DISARMED");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setMotorState(EnumExoStates::DISARMED);
    Serial.println("Motor state: DISARMED");
}

void callbackButtonArmed(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_ARMED");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setMotorState(EnumExoStates::ARMED); 
    Serial.println("Motor state: ARMED");
}

void callbackButtonMotorised(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_MOTORISED");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setMotorState(EnumExoStates::MOTORISED); 
    Serial.println("Motor state: MOTORISED");
}

void callbackButtonWalk(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_WALK");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setProximState(false);
}

void callbackButtonSquat(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_SQUAT");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setProximState(true);
}

void callbackAutocalib(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_AUTOCALIB");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setHeight(400);
}

void callbackBUTTON_4F(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_4F");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setHeight(200);
}

void callbackBUTTON_5F8(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_5F8");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setHeight(400);
}

void callbackBUTTON_5F10(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_5F10");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setHeight(450);
}

void callbackBUTTON_6F(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_6F");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setHeight(500);
}

void callbackBUTTON_PID0(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_PID0 -> Doing nothing XD");
}

void callbackBUTTON_PID1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_PID1 -> Doing nothing XD");
}

void callbackBUTTON_PID2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_PID2 -> Doing nothing XD");
}

void callbackBUTTON_SAVE_PID(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_SAVE_PID");

    NexButton *buttonControl = static_cast<NexButton*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.saveMotorPower();
}

void callbackP_SLIDER(void *ptr){
    Serial.println("Interaction with EnumScreenCallback::P_SLIDER");

    NexSlider *sliderControl = static_cast<NexSlider*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    uint32_t value;
    sliderControl->getValue(&value);

    NexNumber* indicator = TouchScreen::getInstance().getIndicator();
    settings.setFutureMotorPower(value);
    indicator->setValue(value);
}




void setupCallbacks(){
    TouchScreen& screen = TouchScreen::getInstance();
    screen.setCallback(EnumScreenCallback::BUTTON_DISARMED, &callbackButtonDisarmed);
    screen.setCallback(EnumScreenCallback::BUTTON_ARMED, &callbackButtonArmed);
    screen.setCallback(EnumScreenCallback::BUTTON_MOTORISED, &callbackButtonMotorised);
    screen.setCallback(EnumScreenCallback::BUTTON_WALK, &callbackButtonWalk);
    screen.setCallback(EnumScreenCallback::BUTTON_SQUAT, &callbackButtonSquat);
    screen.setCallback(EnumScreenCallback::BUTTON_AUTOCALIB, &callbackAutocalib);
    screen.setCallback(EnumScreenCallback::BUTTON_4F, &callbackBUTTON_4F);
    screen.setCallback(EnumScreenCallback::BUTTON_5F8, &callbackBUTTON_5F8);
    screen.setCallback(EnumScreenCallback::BUTTON_5F10, &callbackBUTTON_5F10);
    screen.setCallback(EnumScreenCallback::BUTTON_6F, &callbackBUTTON_6F);
    screen.setCallback(EnumScreenCallback::BUTTON_PID0, &callbackBUTTON_PID0);
    screen.setCallback(EnumScreenCallback::BUTTON_PID1, &callbackBUTTON_PID1);
    screen.setCallback(EnumScreenCallback::BUTTON_PID2, &callbackBUTTON_PID2);
    screen.setCallback(EnumScreenCallback::P_SLIDER, &callbackP_SLIDER);
}


#endif