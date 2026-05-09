#include <Arduino.h>
#include "HMI_Comm.h"

HMI_Comm hmi;

void setup()
{
    Serial.begin(115200);
    hmi.begin("BioGenius");
}

void loop()
{
    hmi.update();
}