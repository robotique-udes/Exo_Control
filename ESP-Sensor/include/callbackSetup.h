#ifndef CALLBACKSETUP_H
#define CALLBACKSETUP_H


#include <Arduino.h>
#include <Nextion.h>
#include "exoSettings.h"
#include "touchScreen.h"


void callbackButtonToggleMotor(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_MOTOR");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setMotorEnabled(state);
}

void callbackButtonToggleClutch(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_CLUTCH");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setClutchEnabled(state);
}

void callbackButtonInit(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_INIT");
    ExoSettings& settings = ExoSettings::getInstance();

    settings.initialise();
}

void callbackButtonResetEncoder(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_RESET_ENCODER");
    ExoSettings& settings = ExoSettings::getInstance();
    settings.resetEncoder();
}

void callbackButtonAutoCalibProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM");
    ExoSettings& settings = ExoSettings::getInstance();
    settings.setResetProxim(true);
}

void callbackButtonToggleAngleSource(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    ExoSettings& settings = ExoSettings::getInstance();
    settings.setAngleSource(state);
}

void callbackButtonToggleProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_PROXIM");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    ExoSettings& settings = ExoSettings::getInstance();

    settings.setProximEnabled(state);
}

void callbackButtonTest(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TEST");

    //...
}

void callbackButtonDev1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_1");
    Serial.print("Switching brightness settings");
    ExoSettings& settings = ExoSettings::getInstance();
    settings.setBrightness();

}

void callbackButtonDev2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_2");
    //...

}
void callbackButtonToogle1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_1");

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
    Serial.println("About to print value : ...");
    sliderControl->getValue(&value);
    Serial.println(value);
    settings.setMotorPower(value);
}


void setupCallbacks(){
    TouchScreen& screen = TouchScreen::getInstance();
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_MOTOR, &callbackButtonToggleMotor);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_CLUTCH, &callbackButtonToggleClutch);
    screen.setCallback(EnumScreenCallback::BUTTON_INIT, &callbackButtonInit);
    screen.setCallback(EnumScreenCallback::BUTTON_RESET_ENCODER, &callbackButtonResetEncoder);
    screen.setCallback(EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM, &callbackButtonAutoCalibProxim);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE, &callbackButtonToggleAngleSource);
    screen.setCallback(EnumScreenCallback::BUTTON_TOGGLE_PROXIM, &callbackButtonToggleProxim);
    screen.setCallback(EnumScreenCallback::BUTTON_TEST, &callbackButtonTest);
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_1, &callbackButtonDev1);
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_2, &callbackButtonDev2);
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_TOGGLE_1, &callbackButtonToogle1);
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_TOGGLE_2, &callbackButtonToggle2);
    screen.setCallback(EnumScreenCallback::SLIDER_DEV_1, &callbackSliderDev1);
    screen.setCallback(EnumScreenCallback::SLIDER_MOTOR_POWER, &callbackSliderMotorPower);
}


#endif