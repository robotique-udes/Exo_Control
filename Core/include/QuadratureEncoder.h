#ifndef QUADRATURE_ENCODER_H
#define QUADRATURE_ENCODER_H

#include <Arduino.h>
#include <define.h>
#include "enums.h"
#include "dataCore.h"

class QuadratureEncoder
{
public:
    static const int PIN_ENC_HAN_DRO_A = 32;
    static const int PIN_ENC_HAN_DRO_B = 33;
    static const int PIN_ENC_HAN_GAU_A = 14;
    static const int PIN_ENC_HAN_GAU_B = 25;
    static const int PIN_ENC_GEN_DRO_A = 13;
    static const int PIN_ENC_GEN_DRO_B = 12;
    static const int PIN_ENC_GEN_GAU_A = 19;
    static const int PIN_ENC_GEN_GAU_B = 4;

    /**
    * @brief Initialize and set pins
    */
    QuadratureEncoder();

    /**
    * @brief Update value of every encoder in dataCore
    */
    void read();

    /**
    * @brief Initialize interrupts
    */
    static void begin();

    /**
    * @brief Getter for pulse value of a specific encoder
    * @param motor Target motor
    * @return Pulse value
    */
    long getPositionPulses(EnumMotorPosition motor);

    /**
    * @brief Reset pulse value of a specific encoder
    * @param motor Target motor
    */
    void resetPosition(EnumMotorPosition motor);
    
private:
    DataCore& dataCore = DataCore::getInstance();
    static QuadratureEncoder *instance;
    volatile long pulses_han_dro;
    volatile long pulses_han_gau;
    volatile long pulses_gen_dro;
    volatile long pulses_gen_gau;
    int pulsesPerRevolution;

    /**
    * @brief Interrupt right hip
    */
    static void ISR_ENC_HAN_DRO();

    /**
    * @brief Interrupt left hip
    */
    static void ISR_ENC_HAN_GAU();

    /**
    * @brief Interrupt right knee
    */
    static void ISR_ENC_GEN_DRO();

    /**
    * @brief Interrupt left knee
    */
    static void ISR_ENC_GEN_GAU();
    
};

#endif
