#ifndef DATACORE_H
#define DATACORE_H

#include <Arduino.h>
#include <Nextion.h>

#include "enums.h"
#include "relay.h"
#include "QuadratureEncoder.h"

using namespace std;

class DataCore{
    private:

        Relay relais;
        QuadratureEncoder *encodeurPtr;

        //Ins
        bool rightProxiState = false;
        bool leftProxiState = false;


        bool motorEnabled;
        bool clutchEnabled;
        bool proximEnabled;
        bool needResetProxim;
        bool sonarState;
        bool angleSource;
        bool brightness;


        //Outs
        int height;
        float motorPower;

        DataCore();

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

        //from proxim
        int getRightProxi();
        int getLeftProxi();
        void setRightProxi(bool state);
        void setLeftProxi(bool state);

        // Code pour le singleton

        static DataCore *instance;
        static DataCore& getInstance();
        DataCore(const DataCore&) = delete;
        DataCore& operator=(const DataCore&) = delete;
};



#endif 