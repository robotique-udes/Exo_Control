#include "BluetoothDataInterpreter.h"

BluetoothDataInterpreter::BluetoothDataInterpreter(){
    data.stopMotors = true;
    data.height = 171; //Height in cm
    data.weight = 180; //Weight in lbs
}

BluetoothDataInterpreter::~BluetoothDataInterpreter(){}

void BluetoothDataInterpreter::interpretData(String rawString) {
    //Incoming message: contentLength;contentType;content;
    int i = 0;

    //Reads start of message to get message length
    int messageLength;
    for (i; rawString[i] != ';'; i++) { 
        char* lengthString = lengthString + rawString[i];
        messageLength = atoi(lengthString);
    }

    //Reads after the first ';' to get the type of content
    int contentType;
    for (i + 1; rawString[i] != ';'; i++) {
        char* contentTypeString = contentTypeString + rawString[i];
        contentType = atoi(contentTypeString);
    }

    //Reads after the second ';' to get the content
    int content;
    for (i + 1; rawString[i] != ';'; i++) {
        char* contentTypeString = contentTypeString + rawString[i];
        contentType = atoi(contentTypeString);
    }


    if (contentType == MOTOR_STATE) {

    }
    else if (contentType == HEIGHT) {

    } 
    else if (contentType == WEIGHT) {

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