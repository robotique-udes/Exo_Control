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
    DataCore &dataCore = DataCore::getInstance();

public:
    /**
     * @brief Instantiate both proximity sensor
     */
    ProxiHandler();

    /**
     * @brief Read from every proxim and write their state to dataCore
     */
    void read();

    /**
     * @brief Auto calibrate trigger distance for both proxims and update brightness settings
     */
    void setTriggerDist();
    /**
     * @brief Print both proximity sensor state (1 if grounded, 0 if not)
     */
    void print();
};

#endif