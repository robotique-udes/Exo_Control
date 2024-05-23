#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>
#include <string>


#include "enumScreenCallbacks.h"

using namespace std;

class TouchScreen{
    private:
        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        // Page Main
        NexDSButton *motorToggle;
        NexDSButton *clutchToggle;
        NexButton *init;

        // Page Calib
        NexButton *resetEncoder;
        NexButton *autoCalibProxim;
        NexSlider *powerSlider;

        // Page debug
        NexDSButton *angleSource;
        NexDSButton *proximToggle;
        NexButton *testExo;

        // Page dev
        NexButton *button1;
        NexButton *button2;
        NexDSButton *toggle1;
        NexDSButton *toggle2;
        NexSlider *slider1;

        // Page log
        NexText *logText;


        NexTouch *listenList[15];

        string currentString;

        TouchScreen();

    public:
        
        static TouchScreen *instance;
        static TouchScreen& getInstance();

        TouchScreen(const TouchScreen&) = delete;
        TouchScreen& operator=(const TouchScreen&) = delete;
        
        NexNumber* getIndicator();
        void update();
        void print(string toPrint);
        void setCallback(EnumScreenCallback callback, void (*callbackFunction)(void *ptr));
};



#endif 