#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include "enum.h"
// #include <MegunoLink.h>
// #include <filter.h>
// #include <CommandHandler.h>
// #include <ArduinoTimer.h>

// PINS MOTEURS
#define D2_IN1_A 5
#define D2_IN2_A 18
#define D2_EN_A 19
#define D2_CT_A A11
// #define D1_IN1_B 9 //Pour prochain PCB
// #define D1_IN2_B 10
// #define D1_EN_B 11
// #define D1_CT_B A6
#define D1_IN1_A 16
#define D1_IN2_A 22
#define D1_EN_A 23
#define D1_CT_A A10
// #define D2_IN1_B 8 //Pour prochain PCB
// #define D2_IN2_B 7
// #define D2_EN_B 6
// #define D2_CT_B A0

// PINS RELAIS
#define RELAIS_PIN_GENOU_GAUCHE 3
#define RELAIS_PIN_GENOU_DROIT 21
#define RELAIS_PIN_HANCHE_GAUCHE 2
#define RELAIS_PIN_HANCHE_DROITE 15

// PINS SONAR
#define TRIG_PIN_GAUCHE 12
#define ECHO_PIN_GAUCHE 39
#define TRIG_PIN_DROIT 13
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
#define LF 0.3        // longueur femur
#define LT 0.2        // longueur tibia
#define G 9.81      // gravité
#define MH 25       // Masse haut du corps/2
#define MF 1      // Masse femur
#define MT 1      // Masse tibia

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
    float PWMRightKnee = 0.0;
    float PWMLeftKnee = 0.0;

    void setPins();
    void beginInterrupt();
    void motorSetSpeed(int ID, int val);
    void readCurrent();
    void CapperFloat(float &val, float max);
    void neededTorque();
    void neededCurrent();
    void PIDCurrent();
    void printData(long Count_pulses);
    void sonarRead(); // return true if grounded, false if not
    void setRelais(int ID, bool state);
    void testRelais();
    void testMotor();
    void setAngle(enumIMU enumType, float val);
};

#endif