#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>
#include <string>

#include "enums.h"

using namespace std;

class TouchScreen
{
private:
    //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
    // Page Main
    NexDSButton *motorToggle;
    NexDSButton *clutchToggle;
    NexButton *init;

    // Page Calib
    NexButton *resetEncoder;
    NexButton *autoCalib;
    NexSlider *powerSlider;

    // Page debug
    NexDSButton *angleSource;
    NexDSButton *groundDetectionToggle;
    NexButton *testExo;

    // Page dev
    NexButton *button1;
    NexButton *button2;
    NexDSButton *toggle1;
    NexDSButton *toggle2;
    NexSlider *slider1;

    // Page log
    NexText *logText;

    int logLines;

    NexTouch *listenList[15];

    string currentString;

    TouchScreen();

public:
    static TouchScreen *instance;
    static TouchScreen &getInstance();
    /**
     * @brief Touchscreen singleton init
     */
    TouchScreen(const TouchScreen &) = delete;
    TouchScreen &operator=(const TouchScreen &) = delete;

    /**
     * @brief Update to next loop for nextion lib
     */
    void update();

    /**
    * @brief Print to HMI log console
    * @param toPrint String to print on console
    */
    void println(string toPrint);

    /**
    * @brief Set all callback for the HMI's buttons and sliders
    * @param callback Callback to set 
    * @param callbackFunction Pointer to callback function
    */
    void setCallback(EnumScreenCallback callback, void (*callbackFunction)(void *ptr));
};

#endif