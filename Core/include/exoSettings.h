#ifndef EXOSETTINGS_H
#define EXOSETTINGS_H

#include <Arduino.h>
#include <Nextion.h>

#include "enumScreenCallbacks.h"
#include "enumAngleSource.h"
#include "enumSonarState.h"
#include "relay.h"
#include "QuadratureEncoder.h"

using namespace std;

class ExoSettings{
    private:

        Relay relais;
        QuadratureEncoder *encodeurPtr;

        
        bool motorEnabled;
        bool clutchEnabled;
        bool proximEnabled;
        bool needResetProxim;
        bool sonarState;
        bool angleSource;
        bool brightness;

        int height;
        float motorPower;

        ExoSettings();

    public:
        

        void init(QuadratureEncoder *encodeur);
        bool isMotorEnabled();
        bool isClutchEnabled();
        bool isProximEnabled();
        void setMotorEnabled(bool setMotorEnabled);
        void setClutchEnabled(bool setClutchEnabled);
        void setProximEnabled(bool setProximEnabled);
        void adjustMotorPower(int powerOffset);
        
        void initialise();

        void resetEncoder();

        void setBrightness();
        bool getBrightness();

        bool getResetProxim();
        void setResetProxim(bool reset);

        bool getAngleSource();
        void setAngleSource(bool setAngleSource);

        bool getSonarState();
        void setSonarState(bool setSonarState);

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