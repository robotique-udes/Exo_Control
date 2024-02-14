#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>
#include "define.h"


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