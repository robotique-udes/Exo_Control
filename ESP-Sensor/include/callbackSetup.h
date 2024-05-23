#ifndef CALLBACKSETUP_H
#define CALLBACKSETUP_H


#include <Arduino.h>
#include <Nextion.h>
#include "exoSettings.h"
#include "enumExoStates.h"
#include "touchScreen.h"


void callbackButtonToggleMotor(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_MOTOR");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setMotorEnabled(state);
}

void callbackButtonToggleClutch(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_CLUTCH");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setClutchEnabled(state);
}

void callbackButtonInit(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_INIT");

    //...
}

void callbackButtonResetEncoder(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_RESET_ENCODER");

    //...
}

void callbackButtonAutoCalibProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM");

    //...
}

void callbackButtonToggleAngleSource(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setAngleSource(state);
}

void callbackButtonToggleProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_PROXIM");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setProximEnabled(state);
}

void callbackButtonTest(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TEST");

    //...
}

void callbackButtonDev1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_1");

    //...
}

void callbackButtonDev2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_2");

    //...
}
void callbackButtonToogle1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_1");

    //...
}

void callbackButtonToggle2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_2");

    //...
}

void callbackSliderDev1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::SLIDER_DEV_1");

    //...
}

void callbackSliderMotorPower(void *ptr){
    Serial.println("Interaction with EnumScreenCallback::SLIDER_MOTOR_POWER");

    NexSlider *sliderControl = static_cast<NexSlider*>(ptr);

    ExoSettings& settings = ExoSettings::getInstance();

    uint32_t value;
    sliderControl->getValue(&value);

    settings.setMotorPower(value);
}


void setupCallbacks(){
    TouchScreen& screen = TouchScreen::getInstance();
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggleMotor);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggleClutch);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonInit);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonResetEncoder);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonAutoCalibProxim);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggleAngleSource);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggleProxim);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonTest);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonDev1);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonDev2);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToogle1);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggle2);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackSliderDev1);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackSliderMotorPower);
}


#endif