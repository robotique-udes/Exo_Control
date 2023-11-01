#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"

using namespace std;

#define RX_HMI 16
#define TX_HMI 17



class TouchScreen
{
    private:
        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        NexButton bDISARMED; // Disarmed
        NexButton bARMED;    // Armed
        NexButton bMOTORISED;// Motorised

        NexButton bWalk; // Standing Button
        NexButton bSquat; // Squatting Button

        NexButton bAutoCalib; // Autocalibrating Button
        NexButton b4f ; // GOOSE
        NexButton b5f8;
        NexButton b5f10;
        NexButton b6f ;  

        NexButton bPID0;
        NexButton bPID1;
        NexButton bPID2;
        NexButton bSavePID;

        NexSlider Pslider;
        NexNumber Pindicator;

    public:
        
        TouchScreen();
        ~TouchScreen();

        void init();
        void update();
        void setCallback(EnumScreenCallback callback, void (*callbackFunction)());
};



#endif 