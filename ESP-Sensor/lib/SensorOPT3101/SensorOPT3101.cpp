#include "SensorOPT3101.h"

// Date constructor
SensorOPT3101::SensorOPT3101(int FrameTiming, int sda, int scl)
{
    Wire.begin(sda, scl);
    Serial.println("before core init");
    CoreSensor.init();

    if(CoreSensor.getLastError()) {
        Serial.println("Failed to init OPT3101 CoreSensor: error");
        Serial.println(CoreSensor.getLastError());
    }

    CoreSensor.setFrameTiming(FrameTiming); //FrameTiming need to be power of 2  and between 1 & 4096 (each sample is 0.25ms long)

    CoreSensor.setChannel(0);

    CoreSensor.setBrightness(OPT3101Brightness::Adaptive);
}

int SensorOPT3101::getChannel() { 
    return CoreSensor.channelUsed;
}

void SensorOPT3101::printData(){
    Serial.print('channelUsed:');
    Serial.println(CoreSensor.channelUsed);
    Serial.print('brightnessUsed:');
    Serial.println((uint8_t)CoreSensor.brightnessUsed);
    Serial.print('ambient:');
    Serial.println(CoreSensor.ambient); //niveau de lumière ambiante dans le dernier sample 
    Serial.print('amplitude:');
    Serial.println(CoreSensor.amplitude); //Intensité de la lumière refléchie 
    Serial.print("distanceMillimeters:");
    Serial.println(minimumDistance); //Distance en millimètres 

}
void SensorOPT3101::printDistance(){
    Serial.print("distanceMillimeters:");
    Serial.println(minimumDistance); //Distance en millimètres 
}

int SensorOPT3101::GetMinDistance(){
    CoreSensor.sample();
    int16_t min= CoreSensor.distanceMillimeters;
    Serial.println();
    Serial.print("0: ");
    Serial.println(CoreSensor.distanceMillimeters);

    for (int i=0; i<2; i++){
        CoreSensor.nextChannel();
        CoreSensor.sample();
        Serial.print(i+1);
        Serial.print(": ");
        int16_t temp_dist = CoreSensor.distanceMillimeters;
        Serial.println(temp_dist);
        if (CoreSensor.distanceMillimeters<min){
            min=CoreSensor.distanceMillimeters;
        }
    }
    CoreSensor.nextChannel();
    minimumDistance = min;
    return min;
}

void SensorOPT3101::SetFrameTiming(int FrameTiming){
    CoreSensor.setFrameTiming(FrameTiming);
    
}

bool SensorOPT3101::IsOnTheGround(){
    int dist = GetMinDistance();
    return dist<TriggerDistance;
}

void SensorOPT3101::SetTriggerDistance(){
    TriggerDistance=GetMinDistance() + GROUND_DISTANCE_RANGE;
}

int SensorOPT3101::GetTriggerDistance(){
    return TriggerDistance;
}