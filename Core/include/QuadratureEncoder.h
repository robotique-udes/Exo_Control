#ifndef QUADRATURE_ENCODER_H
#define QUADRATURE_ENCODER_H

#include <Arduino.h>
#include <define.h>
#include "enums.h"
#include "dataCore.h"

class QuadratureEncoder
{
public:
    QuadratureEncoder();
    void read();
    static void begin();
    long getPositionPulses(EnumMotorPosition motor);
    float getPositionAngle(EnumMotorPosition motor);
    float getPositionAngleRad(EnumMotorPosition motor);
    void resetPosition(EnumMotorPosition motor);
    static const int PIN_ENC_HAN_DRO_A = 32;
    static const int PIN_ENC_HAN_DRO_B = 33;
    static const int PIN_ENC_HAN_GAU_A = 14;
    static const int PIN_ENC_HAN_GAU_B = 25;
    static const int PIN_ENC_GEN_DRO_A = 13;
    static const int PIN_ENC_GEN_DRO_B = 12;
    static const int PIN_ENC_GEN_GAU_A = 19;
    static const int PIN_ENC_GEN_GAU_B = 4;


private:
    DataCore& dataCore = DataCore::getInstance();
    static void ISR_ENC_HAN_DRO();
    static void ISR_ENC_HAN_GAU();
    static void ISR_ENC_GEN_DRO();
    static void ISR_ENC_GEN_GAU();
    static QuadratureEncoder *instance;
    volatile long pulses_han_dro;
    volatile long pulses_han_gau;
    volatile long pulses_gen_dro;
    volatile long pulses_gen_gau;
    int pulsesPerRevolution;
};

#endif
