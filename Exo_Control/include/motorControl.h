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
    public:
        Motor();
        ~Motor();

        

        void beginInterrupt();
        void motorSetSpeed(int val);
        void motorOn();
        void motorOff();
        bool stopMotor(int cap);
        float ReadCurrent();

};

#endif 