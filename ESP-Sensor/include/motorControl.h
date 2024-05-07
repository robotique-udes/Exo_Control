#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include "enum.h"
#include "sonar.h"
#include "define.h"
#include <PinExtender.h>




// PINS MOTEURS
// TODO : change pins for the new PCB
// Pin on PWM extender
#define D2_IN1_A 17
#define D2_IN2_A 21
#define D1_IN1_B 23
#define D1_IN2_B 25
#define D1_IN1_A 13
#define D1_IN2_A 14
#define D2_IN1_B 27
#define D2_IN2_B 32

// Pin on GPIO extender
#define D2_EN_A 22
#define D1_EN_B 26
#define D1_EN_A 16
#define D2_EN_B 33


#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3

class Motor
{
private:
    float CTval = 0.0;
    int Bouton1 = digitalRead(31);
    int Bouton2 = digitalRead(34);
    float Acurrent;
    float current = 0.0;
    int Rotation = 1;
    float ErrorCurrentRightKnee = 0.0;
    float ErrorCurrentLeftKnee = 0.0;
    float IntegralRightKnee = 0.0;
    float IntegralLeftKnee = 0.0;
    float DerivativeRightKnee = 0.0;
    float DerivativeLeftKnee = 0.0;
    float PreviousErrorRightKnee = 0.0;
    float PreviousErrorLeftKnee = 0.0;   

    //Torques 
    float LeftHipTorque = 0.0;
    float RightHipTorque = 0.0;
    float LeftKneeTorque = 0.0;
    float RightKneeTorque = 0.0;
    //IMU angles
    float RightHipAngle = 0.0;
    float RightKneeAngle = 0.0;
    float LeftHipAngle = 0.0;
    float LeftKneeAngle = 0.0;

    //Currents
    float RightKneeNeededCurrent = 0.0;
    float LeftKneeNeededCurrent = 0.0;
    float RightKneeMeasuredCurrent = 0.0;
    float LeftKneeMeasuredCurrent = 0.0;
    float RightHipMeasuredCurrent = 0.0;
    float LeftHipMeasuredCurrent = 0.0;

    // control sonar
    Sonar sonar;


public:
    Motor();
    ~Motor();
    
    int PWMRightKnee = 0;
    int PWMLeftKnee = 0;
    int PWMRightHip = 0;
    int PWMLeftHip = 0;
    
    void sonarRead();
    void setPins();
    void readCurrent();
    void LimitMinMaxFloat(float &val, float max);
    void LimitMinMaxInt(int &val, int max);
    void neededTorque();
    void neededCurrent();
    void PIDCurrent();
    void PIDCurrentPrealable();
    void sendCommand();
    void setMotorMode(bool state);
    void setPower(double p);
    double getPower();

    void setAngle(enumIMU enumType, float val);
    float toDegrees(float radians);
    void printSonar();
    void printTorque();

    void setSonarState(bool state);
    void setHeight(double h);
    double sonarScanR();
    double sonarScanL();

    void motorSetSpeed(int ID, int in1, int in2, int val);
};

#endif