#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include <MegunoLink.h>
#include <filter.h>
#include <CommandHandler.h>
#include <ArduinoTimer.h>


#define IN1  6
#define IN2 5
#define EN 4
#define CT A10
#define ENCB 3 
#define ENCA 2
#define K_RESSORT 0.25
#define DIST_CM 0.245
#define MASSE 0.066
#define PULSE_PAR_TOUR 300
#define TORQUE2CURRENT 6.470
#define KP 0.1
#define KI 0
#define KD 0

#define LH 0.5        // longueur haut du corps
#define LF 0.3        // longueur femur
#define LT 0.2        // longueur tibia
#define LHCM = LH / 2 // longueur haut du corps
#define LFCM = LF / 2 // longueur femur
#define LTCM = LT / 2 // longueur tibia
#define G = 9.81      // gravité
#define MH = 25       // Masse haut du corps/2
#define MF = 30       // Masse femur
#define MT = 15       // Masse tibia

#define ClockWise 1
#define CounterClockWise -1





class Motor 
{
    private:
        
        float CTval=0.0;
        int Bouton1 = digitalRead(31);
        int Bouton2 = digitalRead(34);
        float Acurrent;
        float current=0.0;
        int Rotation = 1;
        float I_ressort = 0.0;
        float I_gravite = 0.0;
        float T_ressort = 0.0;
        float T_gravite = 0.0;
        float CourantSouhaite = 0.0;
        float e = 0.0;
        float angle = 0.0;
        float integral = 0.0;
        float derivative = 0.0;
        float previous_error = 0.0;
        float PWM = 0.0;
    public:
        Motor();
        ~Motor();

        

        void beginInterrupt();
        void motorSetSpeed(int val);
        void motorOn();
        void motorOff();
        bool stopMotor(int cap);
        float ReadCurrent();
        void setAngle(long Count_pulses);
        void CapperFloat(float &val, float max);
        void neededTorque();
        float neededCurrent();
        void printData(long Count_pulses);

};

#endif 