#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"

using namespace std;

class TouchScreen{
    private:
        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        NexButton *bDISARMED;
        NexButton *bARMED;
        NexButton *bMOTORISED;

        NexButton *bWalk;
        NexButton *bSquat;

        NexButton *bAutoCalib;
        NexButton *b4f;
        NexButton *b5f8;
        NexButton *b5f10;
        NexButton *b6f;

        NexButton *bPID0;
        NexButton *bPID1;
        NexButton *bPID2;
        NexButton *bSavePID;

        NexSlider *Pslider;
        NexNumber *Pindicator;

        NexTouch *listenList[17];

        TouchScreen();

    public:
        
        static TouchScreen *instance;
        static TouchScreen& getInstance();

        TouchScreen(const TouchScreen&) = delete;
        TouchScreen& operator=(const TouchScreen&) = delete;
        
        NexNumber* getIndicator();
        void update();
        void setCallback(EnumScreenCallback callback, void (*callbackFunction)(void *ptr));
};



#endif 