#include "HMI_Comm.h"

HMI_Comm::HMI_Comm() {
    data.stopMotors = true;
    data.height = 171;
    data.weight = 77;
}

void HMI_Comm::ServerCallbacks::onConnect(BLEServer* pServer)
{
    owner->deviceConnected = true;
}

void HMI_Comm::ServerCallbacks::onDisconnect(BLEServer* pServer)
{
    owner->deviceConnected = false;
}

void HMI_Comm::CharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic)
{
    String receivedValue = String(pCharacteristic->getValue().c_str());

    if (receivedValue.length() > 0)
    {
        owner->interpretData(receivedValue);
    }
}

void HMI_Comm::begin()
{
    BLEDevice::init(deviceName);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));

    BLEService* pService = pServer->createService(SERVICE_UUID);

    pSendCharacteristic = pService->createCharacteristic(
        SEND_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );

    pReceiveCharacteristic = pService->createCharacteristic(
        RECEIVE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );

    pReceiveCharacteristic->setCallbacks(new CharacteristicCallbacks(this));

    pSendCharacteristic->addDescriptor(new BLE2902());
    pReceiveCharacteristic->addDescriptor(new BLE2902());

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);

    BLEDevice::startAdvertising();

    Serial.println("Waiting a client connection to notify...");
}


void HMI_Comm::update()
{
    checkConnection();
    sendBatteryData(50);
}

void HMI_Comm::checkConnection()
{
    if (!deviceConnected && oldDeviceConnected)
    {
        Serial.println("Device disconnected.");
        //delay(500);
        pServer->startAdvertising();
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected && !oldDeviceConnected)
    {
        oldDeviceConnected = deviceConnected;
        Serial.println("Device Connected");
    }
}

bool HMI_Comm::isConnected() const
{
    return deviceConnected;
}

bool HMI_Comm::getMotorState() const
{
    return data.stopMotors;
}

int HMI_Comm::getHeight() const
{
    return data.height;
}

int HMI_Comm::getWeight() const
{
    return data.weight;
}

void HMI_Comm::setLogic(Logic *logic)
{
    this->logic = logic;
    this->logic = new Logic();
}

void HMI_Comm::setMotorHandler(MotorHandler *handler)
{
    this->motorHandler = handler;
}

void HMI_Comm::interpretData(String rawString) {
    int i = 0;

    int messageLength;
    String lengthString;
    for (; rawString[i] != ';'; i++) {
        lengthString += rawString[i];
    }
    messageLength = atoi(lengthString.c_str());

    int contentType;
    String contentString;
    for (i = i + 1; rawString[i] != ';'; i++) {
        contentString += rawString[i];
    }
    contentType = atoi(contentString.c_str());

    int content;
    int currentLength = 1;
    String messageString;
    for (i = i + 1; messageLength >= currentLength; i++) {
        messageString += rawString[i];
        currentLength++;
    }
    content = atoi(messageString.c_str());

    if (rawString[i] != '$') {
        content = -1;
    }

    if (contentType == MOTOR_STATE) {
        if (content == 1) {
            data.stopMotors = false;
            this->motorHandler->disableMotors();
        }
        else if (content == 2) {
            data.stopMotors = true;
            this->motorHandler->enableMotors();
        }
    }
    else if (contentType == HEIGHT) {
        data.height = content;
    }
    else if (contentType == WEIGHT) {
        data.weight = content;

        //TODO shouldnt be only when the weight arrives
        //idealy both weight and heigth arrives in the same packet
        logic->setMorphology(data.height, data.weight);
    } 

    Serial.print("Message received :o : ");
    Serial.println(rawString);

    Serial.print("Motor state: ");
    Serial.println(getMotorState());

    Serial.print("Height: ");
    Serial.println(getHeight());

    Serial.print("Weight: ");
    Serial.println(getWeight());
}

void HMI_Comm::sendBatteryData(int batteryCharge) {

    if (!deviceConnected)
        return;

    pSendCharacteristic->setValue(String(batteryCharge).c_str());
    pSendCharacteristic->notify();
}