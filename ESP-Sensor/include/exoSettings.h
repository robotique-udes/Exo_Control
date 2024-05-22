#ifndef EXOSETTINGS_H
#define EXOSETTINGS_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"
#include "enumExoStates.h"
#include "relay.h"

using namespace std;

class ExoSettings{
    private:

        // Disarmed, Armed, Motorised
        EnumExoStates currentState;
        
        // Binary states
        bool state = 1;
        bool angleSource = 1;

        // Calibrated height
        int height;

        // Ajusted PID
        int currentMotorPower;
        int futureMotorPower;

        ExoSettings();

    public:
        Relay relais;
        EnumExoStates getMotorState();
        bool getProximState();
        bool getAngleSource();
        int getHeight();
        int getMotorPower();

        void setMotorState(EnumExoStates newState);
        void setProximState(bool setWalk);
        void setAngleSource(bool angleSource);
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