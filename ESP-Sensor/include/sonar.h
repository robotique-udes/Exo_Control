#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>

// PINS SONAR
#define TRIG_PIN_GAUCHE 15
#define ECHO_PIN_GAUCHE 39
#define TRIG_PIN_DROIT 23
#define ECHO_PIN_DROIT 35

#define UNCERTAINTY 0.1

#define LOW               0x0
#define HIGH              0x1


class Sonar
{
private:
    int iteration = 40;

public:
    Sonar();
    ~Sonar();
    void setHeight(double h);
    double getHeight();
    double sonarScanR();
    double sonarScanL();
    void setSonarState(bool state);
    void sonarRead();
    bool getSonarStateL();
    bool getSonarStateR();
};

#endif