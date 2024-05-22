#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>


// PINS MOTEURS
#define D2_IN1_A 17
#define D2_IN2_A 21
#define D2_EN_A 22
#define D1_IN1_B 23 //Pour prochain PCB
#define D1_IN2_B 25
#define D1_EN_B 26
#define D1_IN1_A 13
#define D1_IN2_A 14
#define D1_EN_A 16
#define D2_IN1_B 27 //Pour prochain PCB
#define D2_IN2_B 32
#define D2_EN_B 33

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3


class Motor 
{
    private:
        
        float PWM = 0.0;

    public:
        Motor();
        ~Motor();
        void begin();

        void motorSetSpeed(int ID, int in1, int in2, int val);
        void motorTestExtender();
};

#endif 