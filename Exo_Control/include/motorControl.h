#include <Arduino.h>
#include <MegunoLink.h>
#include <filter.h>
#include <CommandHandler.h>
#include <ArduinoTimer.h>


#define IN1  4

#define IN2 3

#define EN 2

#define CT A11

ExponentialFilter<float> FilteredCurrent(7, 0);

class Motor 
{
    private:
        float CTval=0.0;
        int Bouton1 = digitalRead(31);
        int Bouton2 = digitalRead(34);
        float Acurrent;
        float current=0.0;
    public:
        Motor();
        ~Motor();

        bool stopMotor(int cap);

};