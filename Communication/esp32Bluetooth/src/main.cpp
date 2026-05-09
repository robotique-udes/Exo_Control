#include <Arduino.h>
#include "HMI_Comm.h"

HMI_Comm hmi;

void setup()
{
    Serial.begin(115200);
    hmi.begin();
}

void loop()
{
    hmi.update();
    hmi.sendBatteryData(90);
    delay(200);
}