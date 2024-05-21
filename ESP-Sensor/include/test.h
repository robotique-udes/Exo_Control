#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "relay.h"
#include "define.h"
#include "motorControl.h"
#include "QuadratureEncoder.h"

class Test
{
public:
    void testSonar();
    void testRelay();
    void keyboardCommand(); // S'assurer qu'il n'y a pas de délais dans le main pour que les commandes soient réactives
    void setMotor(Motor* motor);
    void setEncoder(QuadratureEncoder* encoder);
private:
    Relay relais;
    Motor* motor;
    QuadratureEncoder* encoder;
    long duration;
    double cm, inches;
    int trigPin = 26;  // Trigger
    int echoPin = 27;  // Echo
    bool state=false;
    double height=40;
    
};

#endif