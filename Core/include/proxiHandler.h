#ifndef PROXIHANDLER_H
#define PROXIHANDLER_H

#include "proxiSensor.h"
#include "settings.h"
#include "define.h"
#include "multiplex.h"

class ProxiHandler
{
private:
    Multiplex mux;
    ProxiSensor *LeftProxim;
    ProxiSensor *RightProxim;
    DataCore& dataCore = DataCore::getInstance();
public:
    ProxiHandler();

    void read();
    void setTriggerDist();
    void print();

};

#endif