#include "BluetoothDataInterpreter.h"

BluetoothDataInterpreter::BluetoothDataInterpreter(){
    data.stopMotors = true;
    data.height = 171; //Height in cm
    data.weight = 77; //Weight in kg
}

BluetoothDataInterpreter::~BluetoothDataInterpreter(){}

void BluetoothDataInterpreter::interpretData(String rawString) {
    //Incoming message: contentLength;contentType;content$
    int i = 0;

    //Reads start of message to get message length
    int messageLength;
    String lengthString;
    for (i; rawString[i] != ';'; i++) { 
        lengthString = lengthString + rawString[i];
    }
    const char* lengthChar = lengthString.c_str();
    messageLength = atoi(lengthChar);
    data.test = messageLength;

    //Reads after the first ';' to get the type of content
    int contentType;
    String contentString;
    for (i + 1; rawString[i] != ';'; i++) {
        contentString = contentString + rawString[i];
    }
    const char* contentChar = contentString.c_str();
    contentType = atoi(contentChar);

    //Reads after the second ';' to get the content
    int content;
    int currentLength = 1;
    String messageString;
    for (i + 1; messageLength > currentLength; i++) {
        messageString = messageString + rawString[i];
        currentLength++;
    }
    const char* messageChar = messageString.c_str();
    content = atoi(messageChar);

    //Checks for end of message symbol and returns -1 if the symbol wasn't found
    if (rawString[i + 1] != '$') {
        content = -1;
    }

    if (contentType == MOTOR_STATE) {
        if (content == 1) {
            data.stopMotors = true;
        }
        else if (content == 2) {
            data.stopMotors = false;
        }
    }
    else if (contentType == HEIGHT) {
        data.height = content;
    } 
    else if (contentType == WEIGHT) {
        data.weight = content;
    }
}

bool BluetoothDataInterpreter::getMotorState() {
    return data.stopMotors;
}

int BluetoothDataInterpreter::getHeight() {
    return data.height;
}

int BluetoothDataInterpreter::getWeight() {
    return data.weight;
}

int BluetoothDataInterpreter::getTest() {
    return data.test;
}

int BluetoothDataInterpreter::getCharTest() {
    return data.charTest;
}