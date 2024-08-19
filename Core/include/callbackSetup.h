#ifndef CALLBACKSETUP_H
#define CALLBACKSETUP_H


#include <Arduino.h>
#include <Nextion.h>
#include "dataCore.h"
#include "touchScreen.h"

TouchScreen& screenForCallbacks = TouchScreen::getInstance();

/**
 * @brief Callback for motor enable button
 */
void callbackButtonToggleMotor(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_MOTOR");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    DataCore& settings = DataCore::getInstance();

    settings.setMotorEnabled(state);

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_MOTOR state : " + to_string(state));
}

/**
 * @brief Callback for clutch enable button
 */
void callbackButtonToggleClutch(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_CLUTCH");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    DataCore& settings = DataCore::getInstance();

    settings.setClutchEnabled(state);

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_CLUTCH state : " + to_string(state));
}

/**
 * @brief Callback for initialisation button
 */
void callbackButtonInit(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_INIT");
    DataCore& settings = DataCore::getInstance();

    settings.initialise();

    screenForCallbacks.println("Pressed EnumScreenCallback::INIT");
}

/**
 * @brief Callback for encoder reset button
 */
void callbackButtonResetEncoder(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_RESET_ENCODER");
    DataCore& settings = DataCore::getInstance();
    settings.resetEncoder();
    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_RESET_ENCODER");
}

/**
 * @brief Callback for proxim's auto calibration button
 */
void callbackButtonAutoCalibProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM");
    DataCore& settings = DataCore::getInstance();
    settings.setResetProxim(true);
    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_AUTO_CALIB_PROXIM");
}

/**
 * @brief Callback for angle source toggle button
 */
void callbackButtonToggleAngleSource(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    DataCore& settings = DataCore::getInstance();
    settings.setAngleSource(state);

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_ANGLE_SOURCE : " + to_string(state));
}

/**
 * @brief Callback for proxim activation button
 */
void callbackButtonToggleProxim(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_PROXIM");

    NexDSButton *button = static_cast<NexDSButton*>(ptr);

    uint32_t state;

    button->getValue(&state);

    DataCore& settings = DataCore::getInstance();

    settings.setProximEnabled(state);

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_TOGGLE_PROXIM : " + to_string(state));
}

/**
 * @brief Placeholder test button if we need
 */
void callbackButtonTest(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_TEST");

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_TEST");
    //...
}

/**
 * @brief Placeholder button if we need (has been assign to proxim's brightness setting)
 */
void callbackButtonDev1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_1");
    Serial.print("Switching brightness settings");
    DataCore& settings = DataCore::getInstance();
    settings.setBrightness();
    
    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_DEV_1");
}

/**
 * @brief Placeholder button if we need
 */
void callbackButtonDev2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_2");

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_DEV_2");
}
/**
 * @brief Placeholder toggle button if we need
 */
void callbackButtonToggle1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_1");

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_1");
}

/**
 * @brief Placeholder toggle button if we need
 */
void callbackButtonToggle2(void *ptr){
    Serial.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_2");

    //...

    screenForCallbacks.println("Pressed EnumScreenCallback::BUTTON_DEV_TOGGLE_2");
}

/**
 * @brief Placeholder slider if we need
 */
void callbackSliderDev1(void *ptr){
    Serial.println("Pressed EnumScreenCallback::SLIDER_DEV_1");

    //...

    screenForCallbacks.println("Pressed EnumScreenCallback::SLIDER_DEV_1");
}

/**
 * @brief Callback for motor power slider
 */
void callbackSliderMotorPower(void *ptr){
    Serial.println("Interaction with EnumScreenCallback::SLIDER_MOTOR_POWER");

    NexSlider *sliderControl = static_cast<NexSlider*>(ptr);

    DataCore& settings = DataCore::getInstance();

    uint32_t value;
    Serial.println("About to print value : ...");
    sliderControl->getValue(&value);
    Serial.println(value);
    settings.setMotorPower(value);

    screenForCallbacks.println("Interaction with EnumScreenCallback::SLIDER_MOTOR_POWER : " + to_string(value));
}

/**
 * @brief Sets every callback for the hmi
 */
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
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_TOGGLE_1, &callbackButtonToggle1);
    screen.setCallback(EnumScreenCallback::BUTTON_DEV_TOGGLE_2, &callbackButtonToggle2);
    screen.setCallback(EnumScreenCallback::SLIDER_DEV_1, &callbackSliderDev1);
    screen.setCallback(EnumScreenCallback::SLIDER_MOTOR_POWER, &callbackSliderMotorPower);
}


#endif