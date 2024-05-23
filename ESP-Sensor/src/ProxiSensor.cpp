#include "ProxiSensor.h"
#include "define.h"

// Date constructor
ProxiSensor::ProxiSensor(Multiplex *muxPtr, int muxAddress)
{
    this->muxAddress = muxAddress;
    this->muxPtr = muxPtr;

    this->muxPtr->selectChannel(muxAddress);
    Serial.print("Mux Channel: ");
    Serial.println(this->muxPtr->getCurrentChannel());
    CoreSensor.init();

    if (CoreSensor.getLastError())
    {
        Serial.println("Failed to init OPT3101 CoreSensor: error");
        Serial.println(CoreSensor.getLastError());
    }

    CoreSensor.setFrameTiming(PROXIM_REFRESH_RATE); // FrameTiming need to be power of 2  and between 1 & 4096 (each sample is 0.25ms long)

    CoreSensor.setChannel(0);

    CoreSensor.setBrightness(OPT3101Brightness::High);

    SetTriggerDistance();
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
    
    this->muxPtr->selectChannel(muxAddress);
    int16_t min = 0;
    float moyenne = 0;
    int sample = 0;
    //Itère dans chaque channel pour faire une lecture et remplace min si elle est plus petite que la précédante
    for(int j = 0; j<3; j++){
        for (int i = 0; i < 3; i++)
        {
            CoreSensor.setChannel(i);
            CoreSensor.sample();

            int16_t dist = CoreSensor.distanceMillimeters;
            if ((dist < min || min == 0) && dist > 0)
            {
                min = dist;
            }
        }
        if(min != 0){
            moyenne += min;
            sample++;
        }
    }
    CoreSensor.setChannel(0);
    minimumDistance = (moyenne/sample);
    return minimumDistance;
}

void ProxiSensor::SetFrameTiming(int FrameTiming)
{
    this->muxPtr->selectChannel(muxAddress);
    CoreSensor.setFrameTiming(FrameTiming);
}

bool ProxiSensor::IsOnTheGround()
{
    
    int dist = GetMinDistance();
    // Serial.print("\t Min dist: ");
    // Serial.print(minimumDistance);
    // Serial.print("\t Trigger dist: ");
    // Serial.print(TriggerDistance);
    return (dist < TriggerDistance);
}

void ProxiSensor::SetTriggerDistance()
{
    float moyenne;
    for(int i= 0; i<3; i++){
        moyenne = GetMinDistance();
    }
    TriggerDistance = int(moyenne/3)  + GROUND_DISTANCE_RANGE;
    Serial.print("Trigger dist set to: ");
    Serial.println(TriggerDistance);
}

int ProxiSensor::GetTriggerDistance()
{
    return TriggerDistance;
}