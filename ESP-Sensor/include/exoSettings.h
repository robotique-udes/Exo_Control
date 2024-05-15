#ifndef EXOSETTINGS_H
#define EXOSETTINGS_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"
#include "enumExoStates.h"

using namespace std;

class ExoSettings{
    private:

        // Disarmed, Armed, Motorised
        EnumExoStates currentState;
        
        // Tests
        bool state;

        // Calibrated height
        int height;

        // Ajusted PID
        int currentMotorPower;
        int futureMotorPower;

        ExoSettings();

    public:
        
        EnumExoStates getCurrentState();
        bool getState();
        int getHeight();
        int getMotorPower();

        void setCurrentState(EnumExoStates newState);
        void setState(bool setWalk);
        void setHeight(int setHeight);
        void setFutureMotorPower(double setMotoPower);
        void saveMotorPower();

        // Code pour le singleton

        static ExoSettings *instance;
        static ExoSettings& getInstance();
        ExoSettings(const ExoSettings&) = delete;
        ExoSettings& operator=(const ExoSettings&) = delete;
};



#endif 