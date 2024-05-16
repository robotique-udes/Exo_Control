#include <Arduino.h>

const int PIN_ENC_HAN_DRO_A = 32;
const int PIN_ENC_HAN_DRO_B = 33;
const int PIN_ENC_HAN_GAU_A = 14;
const int PIN_ENC_HAN_GAU_B = 25;
const int PIN_ENC_GEN_DRO_A = 13;
const int PIN_ENC_GEN_DRO_B = 12;
const int PIN_ENC_GEN_GAU_A = 19;
const int PIN_ENC_GEN_GAU_B = 4;
const int PULSES_PER_REVOLUTION = 2442.96;

volatile long pulses_han_dro = 0;
volatile long pulses_han_gau = 0;
volatile long pulses_gen_dro = 0;
volatile long pulses_gen_gau = 0;
volatile long pulse_test = 0;
int pulsesPerRevolution = PULSES_PER_REVOLUTION;

void ISR_ENC_HAN_DRO() {
    pulses_han_dro += (digitalRead(PIN_ENC_HAN_DRO_B)) ? 1 : -1;
}

void ISR_ENC_HAN_GAU() {
    pulses_han_gau += (digitalRead(PIN_ENC_HAN_GAU_B)) ? 1 : -1;
}

void ISR_ENC_GEN_DRO() {
    pulses_gen_dro += (digitalRead(PIN_ENC_GEN_DRO_B)) ? 1 : -1;
}

void ISR_ENC_GEN_GAU() {
    pulse_test++;
    pulses_gen_gau += (digitalRead(PIN_ENC_GEN_GAU_B)) ? 1 : -1;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Encoder test!");
    pinMode(PIN_ENC_HAN_DRO_A, INPUT);
    pinMode(PIN_ENC_HAN_DRO_B, INPUT);
    pinMode(PIN_ENC_HAN_GAU_A, INPUT);
    pinMode(PIN_ENC_HAN_GAU_B, INPUT);
    pinMode(PIN_ENC_GEN_DRO_A, INPUT);
    pinMode(PIN_ENC_GEN_DRO_B, INPUT);
    pinMode(PIN_ENC_GEN_GAU_A, INPUT);
    pinMode(PIN_ENC_GEN_GAU_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_DRO_A), ISR_ENC_HAN_DRO, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_HAN_GAU_A), ISR_ENC_HAN_GAU, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_DRO_A), ISR_ENC_GEN_DRO, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_GEN_GAU_A), ISR_ENC_GEN_GAU, RISING);
}

void loop() {
    // Your main code goes here
    Serial.print("Position Hanche Droite: ");
    Serial.println(pulses_han_dro);
    Serial.print("Position Hanche Gauche: ");
    Serial.println(pulses_han_gau);
    Serial.print("Position Genou Droit: ");
    Serial.println(pulses_gen_dro);
    Serial.print("Position Genou Gauche: ");
    Serial.println(pulses_gen_gau);
    Serial.print("Pulse test: ");
    Serial.println(pulse_test);
    delay(1000);
}
