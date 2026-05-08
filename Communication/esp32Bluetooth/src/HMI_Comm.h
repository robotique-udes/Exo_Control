#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BluetoothDataInterpreter.h"

#define SERVICE_UUID        "19b10000-e8f2-537e-4f6c-d104768a1214"
#define HMI_CHARACTERISTIC_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"

class HMI_Comm
{
    private:
        BLEServer* pServer = NULL;
        BLECharacteristic* pHMICharacteristic = NULL;
        bool deviceConnected = false;
        bool oldDeviceConnected = false;

        



    public:

    void start();
}