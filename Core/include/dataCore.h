#ifndef DATACORE_H
#define DATACORE_H

#include <Arduino.h>
#include <Nextion.h>

#include "define.h"
#include "enums.h"
#include "relay.h"

using namespace std;

class DataCore{
    private:

        Relay relais;

        //PWM
        int PWMRightKnee = 0;
        int PWMLeftKnee = 0;
        int PWMRightHip = 0;
        int PWMLeftHip = 0;

        //Ins
        //proxim
        bool rightProxiState = false;
        bool leftProxiState = false;
        bool proximEnabled;
        bool needResetProxim;

        //encoder
        float encoder_knee_right = 0;
        float encoder_knee_left = 0;
        float encoder_hip_right = 0;
        float encoder_hip_left = 0;
        bool needResetEncoder = false;

        //BNO
        float Imu_knee_right = 0;
        float Imu_knee_left = 0;
        float Imu_hip_right = 0;
        float Imu_hip_left = 0;
        float Imu_back = 0;


        bool motorEnabled;
        bool clutchEnabled;
        bool sonarState;
        bool angleSource;
        bool brightness;


        //Outs
        int height;
        float motorPower;

        DataCore();

    public:
        

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

        int getMotorPower();
        void setMotorPower(int setMotorPower);

        //proxim
        int getRightProxi();
        int getLeftProxi();
        void setRightProxi(bool state);
        void setLeftProxi(bool state);

        //encoder
        float getEncoderDeg(EnumMotorPosition motor);
        float getEncoderRad(EnumMotorPosition motor);
        void setEncoderAngles(EnumMotorPosition motor, int pulse);
        bool isEncoderResetNeeded();
        void setEncoderReset(bool state);

        //Bno
        void setBnoAngles(EnumBnoPosition bno, float angle);
        float getBnoAngles(EnumBnoPosition bno);

        //PWM
        void setPWM(EnumMotorPosition motor, float pwm);
        float getPWM(EnumMotorPosition motor);

        // Code pour le singleton

        static DataCore *instance;
        static DataCore& getInstance();
        DataCore(const DataCore&) = delete;
        DataCore& operator=(const DataCore&) = delete;
};



#endif 