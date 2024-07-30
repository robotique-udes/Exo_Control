#include "quadratureEncoder.h"

QuadratureEncoder *QuadratureEncoder::instance = nullptr;

QuadratureEncoder::QuadratureEncoder()
{
    pinMode(PIN_ENC_HAN_DRO_A, INPUT);
    pinMode(PIN_ENC_HAN_DRO_B, INPUT);
    pinMode(PIN_ENC_HAN_GAU_A, INPUT);
    pinMode(PIN_ENC_HAN_GAU_B, INPUT);
    pinMode(PIN_ENC_GEN_DRO_A, INPUT);
    pinMode(PIN_ENC_GEN_DRO_B, INPUT);
    pinMode(PIN_ENC_GEN_GAU_A, INPUT);
    pinMode(PIN_ENC_GEN_GAU_B, INPUT);
    this->pulsesPerRevolution = PULSES_PER_REVOLUTION;
    instance = this;
    pulses_han_dro = 0;
    pulses_han_gau = 0;
    pulses_gen_dro = 0;
    pulses_gen_gau = 0;

}

void QuadratureEncoder::begin()
{
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_DRO_A), &QuadratureEncoder::ISR_ENC_HAN_DRO, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_GAU_A), &QuadratureEncoder::ISR_ENC_HAN_GAU, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_DRO_A), &QuadratureEncoder::ISR_ENC_GEN_DRO, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_GAU_A), &QuadratureEncoder::ISR_ENC_GEN_GAU, RISING);

}

void QuadratureEncoder::read()
{
    if(dataCore.isEncoderResetNeeded())
    {
        dataCore.setEncoderReset(false);
        pulses_han_dro=0;
        pulses_han_gau=0;
        pulses_gen_dro=0;
        pulses_gen_gau=0;

    }
    else{
        dataCore.setEncoderAngles(EnumMotorPosition::HIP_R, pulses_han_dro);
        dataCore.setEncoderAngles(EnumMotorPosition::HIP_L, pulses_han_gau);
        dataCore.setEncoderAngles(EnumMotorPosition::HIP_R, pulses_gen_dro);
        dataCore.setEncoderAngles(EnumMotorPosition::HIP_R, pulses_gen_gau);
    }

}

long QuadratureEncoder::getPositionPulses(EnumMotorPosition motor)
{
    Serial.println("getPositionPulses");
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        return pulses_han_dro;
    case EnumMotorPosition::HIP_L:
        return pulses_han_gau;
    case EnumMotorPosition::KNEE_R:
        return pulses_gen_dro;
    case EnumMotorPosition::KNEE_L:
        return pulses_gen_gau;
    default:
        Serial.println("Invalid motor");
        return 0;
    }
}

void QuadratureEncoder::ISR_ENC_HAN_DRO()
{
    if (instance != nullptr)
    {
        instance->pulses_han_dro += (digitalRead(PIN_ENC_HAN_DRO_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_HAN_GAU()
{
    if (instance != nullptr)
    {
        instance->pulses_han_gau += (digitalRead(PIN_ENC_HAN_GAU_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_GEN_DRO()
{
    if (instance != nullptr)
    {
        instance->pulses_gen_dro += (digitalRead(PIN_ENC_GEN_DRO_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_GEN_GAU()
{
    if (instance != nullptr)
    {
        instance->pulses_gen_gau += (digitalRead(PIN_ENC_GEN_GAU_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::resetPosition(EnumMotorPosition motor)
{
    switch (motor)
    {
    case EnumMotorPosition::HIP_R:
        pulses_han_dro = 0;
        break;
    case EnumMotorPosition::HIP_L:
        pulses_han_gau = 0;
        break;
    case EnumMotorPosition::KNEE_R:
        pulses_gen_dro = 0;
        break;
    case EnumMotorPosition::KNEE_L:
        pulses_gen_gau = 0;
        break;
    default:
        Serial.println("Invalid motor");
    }
}