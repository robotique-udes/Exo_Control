#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "relay.h"
#include "define.h"

class Test
{
public:
    void testSonar();
    void testRelay();
private:
    Relay relais;
    long duration;
    double cm, inches;
    int trigPin = 26;  // Trigger
    int echoPin = 27;  // Echo
    bool state=false;
    double height=40;
    
};

#endif