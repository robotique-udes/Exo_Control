#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "relay.h"
#include "define.h"
#include "motorControl.h"

class Test
{
public:
    void testSonar();
    void testRelay();
    void keyboardCommand();
    void setMotor(Motor* motor);
private:
    Relay relais;
    Motor* motor;
    long duration;
    double cm, inches;
    int trigPin = 26;  // Trigger
    int echoPin = 27;  // Echo
    bool state=false;
    double height=40;
    
};

#endif