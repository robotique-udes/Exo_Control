#ifndef TEST_H
#define TEST_H

#include <Arduino.h>
#include "relay.h"
#include "define.h"
#include "motor.h"
#include "QuadratureEncoder.h"
#include "bnoHandler.h"
#include "dataCore.h"
#include "logic.h"

class Test
{
public:

    /**
    * @brief Toggle every relay on/off
    */
    void testRelay();

    /**
    * @brief Enable multiple keyboard inpt for testing
    */
    void keyboardCommand(); // S'assurer qu'il n'y a pas de délais dans le main pour que les commandes soient réactives

    /**
    * @brief Set motor pointer for testing
    * @param motor Pointer to motor
    */
    void setMotor(Motor* motor);

    /**
    * @brief Set encoder pointer for testing
    * @param encoder Pointer to encoder
    */
    void setEncoder(QuadratureEncoder* encoder);

    void setBNOHandler(BnoHandler *newHandler);

    void setLogic(Logic *newLogic);

private:
    Relay relais;
    Motor* motor;
    QuadratureEncoder* encoder;
    BnoHandler *bnoHandler;
    Logic   *logicHandler;
    bool motorState = false;
    long duration;
    double cm, inches;
    bool state=false;
    double height=40;
    //DataCore instanciation
    DataCore& dataCore = DataCore::getInstance();
    
};

#endif