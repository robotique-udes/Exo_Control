#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Logic.hpp"
#include "MotorHandler.hpp"

#define SERVICE_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SEND_CHARACTERISTIC_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"
#define RECEIVE_CHARACTERISTIC_UUID "19b10002-e8f2-537e-4f6c-d104768a1214"

class HMI_Comm
{
private:
    BLECharacteristic* pSendCharacteristic = nullptr;
    BLECharacteristic* pReceiveCharacteristic = nullptr;

    Logic* logic;
    MotorHandler* motorHandler;

    const char* deviceName = "BioGenius";

    uint32_t value = 0;

    enum DataType {
        MOTOR_STATE = 1,
        HEIGHT = 2,
        WEIGHT = 3,
    };

    struct Data
    {
        bool stopMotors;
        int height;
        int weight;
    };

    Data data;

    class ServerCallbacks : public BLEServerCallbacks
    {
    private:
        HMI_Comm* owner;

    public:
        ServerCallbacks(HMI_Comm* ownerPtr) : owner(ownerPtr) {}

        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
    };

    class CharacteristicCallbacks : public BLECharacteristicCallbacks
    {
    private:
        HMI_Comm* owner;

    public:
        CharacteristicCallbacks(HMI_Comm* ownerPtr) : owner(ownerPtr) {}

        void onWrite(BLECharacteristic* pCharacteristic) override;
    };


    void checkConnection();

public:
    HMI_Comm();

    void begin();
    void update();

    void setLogic(Logic *logic);
    void setMotorHandler(MotorHandler *handler);
    void sendBatteryData(int batteryCharge);
    
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    bool isConnected() const;
    BLEServer* pServer = nullptr;

    bool getMotorState() const;
    int getHeight() const;
    int getWeight() const;

    void interpretData(String rawString);
};

