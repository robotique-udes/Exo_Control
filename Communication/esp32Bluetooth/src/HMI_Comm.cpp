#include "HMI_Comm.h"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic* pLedCharacteristic) {

    String value = (pLedCharacteristic->getValue()).c_str();

    if (value.length() > 0) {
        Serial.print("Message received: "); Serial.println(value);
        DataInterpreter.interpretData(value);

        Serial.print("Motor state: "); Serial.println(DataInterpreter.getMotorState());
        Serial.print("Height: "); Serial.println(DataInterpreter.getHeight());
        Serial.print("Weight: "); Serial.println(DataInterpreter.getWeight());
    }
  }
};