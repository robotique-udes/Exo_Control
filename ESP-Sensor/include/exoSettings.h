#ifndef EXOSETTINGS_H
#define EXOSETTINGS_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"
#include "enumAngleSource.h"
#include "enumSonarState.h"
#include "relay.h"

using namespace std;

class ExoSettings{
    private:

        bool motorEnabled;
        bool clutchEnabled;
        bool proximEnabled;

        EnumSonarState sonarState;
        EnumAngleSource angleSource;

        int height;
        int motorPower;

        ExoSettings();

    public:
        Relay relais;

        bool isMotorEnabled();
        bool isClutchEnabled();
        bool isProximEnabled();
        void setMotorEnabled(bool setMotorEnabled);
        void setClutchEnabled(bool setClutchEnabled);
        void setProximEnabled(bool setProximEnabled);

        EnumAngleSource getAngleSource();
        void setAngleSource(EnumAngleSource setAngleSource);

        EnumSonarState getSonarState();
        void setSonarState(EnumSonarState setSonarState);

        int getHeight();
        void setHeight(int setHeight);

        int getMotorPower();
        void setMotorPower(int setMotorPower);

        // Code pour le singleton

        static ExoSettings *instance;
        static ExoSettings& getInstance();
        ExoSettings(const ExoSettings&) = delete;
        ExoSettings& operator=(const ExoSettings&) = delete;
};



#endif 