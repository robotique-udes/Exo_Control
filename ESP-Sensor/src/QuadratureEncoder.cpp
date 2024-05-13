#include "QuadratureEncoder.h"

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

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_DRO_A), ISR_ENC_HAN_DRO, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_DRO_B), ISR_ENC_HAN_DRO, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_GAU_A), ISR_ENC_HAN_GAU, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_GAU_B), ISR_ENC_HAN_GAU, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_DRO_A), ISR_ENC_GEN_DRO, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_DRO_B), ISR_ENC_GEN_DRO, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_GAU_A), ISR_ENC_GEN_GAU, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_GAU_B), ISR_ENC_GEN_GAU, CHANGE);
    this->pulsesPerRevolution = PULSES_PER_REVOLUTION;
    instance = this;
}

void QuadratureEncoder::begin()
{
    pulses_han_dro = 0;
    pulses_han_gau = 0;
    pulses_gen_dro = 0;
    pulses_gen_gau = 0;
}

long QuadratureEncoder::getPositionPulses(int motor)
{
    switch (motor)
    {
    case HAN_DRO:
        return pulses_han_dro;
    case HAN_GAU:
        return pulses_han_gau;
    case GEN_DRO:
        return pulses_gen_dro;
    case GEN_GAU:
        return pulses_gen_gau;
    default:
        Serial.println("Invalid motor");
        return 0;
    }
}

float QuadratureEncoder::getPositionAngle(int motor)
{
    long pulses;
    switch (motor)
    {
    case HAN_DRO:
        pulses = pulses_han_dro;
        break;
    case HAN_GAU:
        pulses = pulses_han_gau;
        break;
    case GEN_DRO:
        pulses = pulses_gen_dro;
        break;
    case GEN_GAU:
        pulses = pulses_gen_gau;
        break;
    default:
        Serial.println("Invalid motor");
        return 0.0;
    }
    return (float)pulses / pulsesPerRevolution * 360.0;
}

void QuadratureEncoder::ISR_ENC_HAN_DRO()
{
    if (instance != nullptr)
    {
        instance->pulses_han_dro += (digitalRead(PIN_ENC_HAN_DRO_A) == digitalRead(PIN_ENC_HAN_DRO_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_HAN_GAU()
{
    if (instance != nullptr)
    {
        instance->pulses_han_gau += (digitalRead(PIN_ENC_HAN_GAU_A) == digitalRead(PIN_ENC_HAN_GAU_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_GEN_DRO()
{
    if (instance != nullptr)
    {
        instance->pulses_gen_dro += (digitalRead(PIN_ENC_GEN_DRO_A) == digitalRead(PIN_ENC_GEN_DRO_B)) ? 1 : -1;
    }
}

void QuadratureEncoder::ISR_ENC_GEN_GAU()
{
    if (instance != nullptr)
    {
        instance->pulses_gen_gau += (digitalRead(PIN_ENC_GEN_GAU_A) == digitalRead(PIN_ENC_GEN_GAU_B)) ? 1 : -1;
    }
}