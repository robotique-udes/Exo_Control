#include "ProxiSensor.h"

// Date constructor
ProxiSensor::ProxiSensor(int FrameTiming, int sda, int scl)
{
    Wire.begin(sda, scl);
    CoreSensor.init();

    if (CoreSensor.getLastError())
    {
        Serial.println("Failed to init OPT3101 CoreSensor: error");
        Serial.println(CoreSensor.getLastError());
    }

    CoreSensor.setFrameTiming(FrameTiming); // FrameTiming need to be power of 2  and between 1 & 4096 (each sample is 0.25ms long)

    CoreSensor.setChannel(0);

    CoreSensor.setBrightness(OPT3101Brightness::Adaptive);
}

int ProxiSensor::GetChannel()
{
    return CoreSensor.channelUsed;
}

void ProxiSensor::SetChannel(int c)
{
    if (c >= 0 and c < 3)
    {
        CoreSensor.setChannel(c);
    }
    else
    {
        Serial.println("Invalid chanel");
    }
}

void ProxiSensor::PrintData()
{
    Serial.print("channelUsed:");
    Serial.println(CoreSensor.channelUsed);
    Serial.print("brightnessUsed:");
    Serial.println((uint8_t)CoreSensor.brightnessUsed);
    Serial.print("ambient:");
    Serial.println(CoreSensor.ambient); // niveau de lumière ambiante dans le dernier sample
    Serial.print("amplitude:");
    Serial.println(CoreSensor.amplitude); // Intensité de la lumière refléchie
    Serial.print("distanceMillimeters:");
    Serial.println(minimumDistance); // Distance en millimètres
}
void ProxiSensor::PrintDistance()
{
    Serial.print("distanceMillimeters:");
    Serial.println(minimumDistance); // Distance en millimètres
}

int ProxiSensor::GetMinDistance()
{
    int16_t min = -1;
    //Itère dans chaque channel pour faire une lecture et remplace min si elle est plus petite que la précédante
    for (int i = 0; i < 3; i++)
    {
        CoreSensor.setChannel(i);
        CoreSensor.sample();

        int16_t dist = CoreSensor.distanceMillimeters;
        if (dist < min || min == -1)
        {
            min = dist;
        }
    }
    CoreSensor.setChannel(0);
    minimumDistance = min;
    return min;
}

void ProxiSensor::SetFrameTiming(int FrameTiming)
{
    CoreSensor.setFrameTiming(FrameTiming);
}

bool ProxiSensor::IsOnTheGround()
{
    int dist = GetMinDistance();
    return (dist < TriggerDistance);
}

void ProxiSensor::SetTriggerDistance()
{
    TriggerDistance = GetMinDistance() + GROUND_DISTANCE_RANGE;
}

int ProxiSensor::GetTriggerDistance()
{
    return TriggerDistance;
}