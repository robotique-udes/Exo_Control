#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include "enum.h"

#define D2_CT_A A7
#define D1_CT_B A0
#define D1_CT_A A6
#define D2_CT_B A3

// PINS RELAIS
#define RELAIS_PIN_GENOU_GAUCHE 21
#define RELAIS_PIN_GENOU_DROIT 22
#define RELAIS_PIN_HANCHE_GAUCHE 13
#define RELAIS_PIN_HANCHE_DROITE 4

// PINS SONAR
#define TRIG_PIN_GAUCHE 15
#define ECHO_PIN_GAUCHE 39
#define TRIG_PIN_DROIT 23
#define ECHO_PIN_DROIT 35

// CONSTANTES
#define K_RESSORT 0.25
#define DIST_CM 0.245
#define MASSE 0.366
#define PULSE_PAR_TOUR 300
#define TORQUE2CURRENT 0.6 // 6.470
#define KP 2.5
#define KI 0
#define KD 0

// LONGUEURS
#define LH 0.5        // longueur haut du corps
#define LF 0.4        // longueur femur
#define LT 0.4        // longueur tibia
#define G 9.81      // gravité
#define MH 25.0       // Masse haut du corps/2
#define MF 3.0      // Masse femur
#define MT 3.0      // Masse tibia

#define ClockWise 1
#define CounterClockWise -1

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3
//#define SONAR_GAUCHE 0
//#define SONAR_DROIT 1

#define RELAIS_GENOU_GAUCHE 0
#define RELAIS_GENOU_DROIT 1
#define RELAIS_HANCHE_GAUCHE 2
#define RELAIS_HANCHE_DROITE 3

#define ON 1
#define OFF 0

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
    float LeftHipTorque = 0.0;
    float RightHipTorque = 0.0;
    float LeftKneeTorque = 0.0;
    float RightKneeTorque = 0.0;
    float RightHipAngle = 0.0;
    float RightKneeAngle = 0.0;
    float LeftHipAngle = 0.0;
    float LeftKneeAngle = 0.0;
    float RightKneeNeededCurrent = 0.0;
    float LeftKneeNeededCurrent = 0.0;
    float RightKneeMeasuredCurrent = 0.0;
    float LeftKneeMeasuredCurrent = 0.0;

    // control sonar
    long RightDuration;
    long LeftDuration;
    double RightCm, LeftCm;
    bool RightSonarState, LeftSonarState = false;
    double height = 40;
    int iteration = 20;

public:
    Motor();
    ~Motor();
    int PWMRightKnee = 0;
    int PWMLeftKnee = 0;
    int PWMRightHip = 0;
    int PWMLeftHip = 0;

    void setPins();
    void beginInterrupt();
    void readCurrent();
    void CapperFloat(float &val, float max);
    void CapperInt(int &val, int max);
    void neededTorque();
    void neededCurrent();
    void PIDCurrent();
    void PIDCurrentPrealable();
    void printData(long Count_pulses);
    void sonarRead(); // return true if grounded, false if not
    void setRelais(int ID, bool state);
    void setAllRelais(bool state);
    void testRelais();
    void testMotor();
    void setAngle(enumIMU enumType, float val);
    float toDegrees(float radians);
    void printSonar();
    void printTorque();

};

#endif