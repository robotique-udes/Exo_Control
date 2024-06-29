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

long QuadratureEncoder::getPositionPulses(int motor)
{
    Serial.println("getPositionPulses");
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
        pulses = -pulses_han_gau;
        break;
    case GEN_DRO:
        pulses = -pulses_gen_dro;
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


float QuadratureEncoder::getPositionAngleRad(int motor)
{
    long pulses;
    switch (motor)
    {
    case HAN_DRO:
        pulses = pulses_han_dro;
        break;
    case HAN_GAU:
        pulses = -pulses_han_gau;
        break;
    case GEN_DRO:
        pulses = -pulses_gen_dro;
        break;
    case GEN_GAU:
        pulses = pulses_gen_gau;
        break;
    default:
        Serial.println("Invalid motor");
        return 0.0;
    }
    return (float)pulses / pulsesPerRevolution * 2 * PI;
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

void QuadratureEncoder::resetPosition(int motor)
{
    switch (motor)
    {
    case HAN_DRO:
        pulses_han_dro = 0;
        break;
    case HAN_GAU:
        pulses_han_gau = 0;
        break;
    case GEN_DRO:
        pulses_gen_dro = 0;
        break;
    case GEN_GAU:
        pulses_gen_gau = 0;
        break;
    default:
        Serial.println("Invalid motor");
    }
}