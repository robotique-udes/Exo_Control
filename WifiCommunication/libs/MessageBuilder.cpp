
#include "messageBuilder.h"

void MessageBuilder::clearMessage()
{
    for (int i = 0; i < MESSAGE_LENGTH; i++)
    {
        message[i] = 0;
    }
}

void MessageBuilder::clearInfo()
{
    for (int i = 0; i < NB_BNO_ANGLE; i++)
    {
        bnoAngle[i].ID = EnumBnoAngle::NONE;  
        bnoAngle[i].value = 0;
    }
    for (int i = 0; i < NB_BNO_POSITION; i++)
    {
        bnoPosition[i].ID = EnumBnoPosition::NONE;
        bnoPosition[i].value = 0;
    }
    for (int i = 0; i < NB_MOTOR_POSITION; i++)
    {
        motorPosition[i].ID = EnumMotorPosition::NONE;
        motorPosition[i].value = 0;
    }
    for (int i = 0; i < LOG_LENGTH; i++)
    {
        logMessage[i] = 0;
    }
}

void MessageBuilder::add(unsigned char log[LOG_LENGTH - 1])
{   
    int indexStart = 0;
    bool logFull = true;
    for (indexStart; indexStart < LOG_LENGTH - 1; indexStart++) // Find first empty index
    {
        if (log[indexStart] == '\0')
        {
            logFull = false;
            indexStart++; // Start writing after the null character
            break;
        }
    }
    if (!logFull) // Check if log is full
    {
        int indexLog = 0;
        for (indexStart; indexStart < LOG_LENGTH - 2; indexStart++) // Write log
        {
            logMessage[indexStart] = log[indexLog];
            if (log[indexLog] == '\0') // Check if end of log
            {
                break;
            }
            indexLog++;
        
        }
        logMessage[LOG_LENGTH - 1] = '\0'; // Add null character to end of log
    }
    logPlace = LOG_LENGTH - indexStart - 2; // Calculate remaining space in log
}

int MessageBuilder::getLogPlace()
{
    return logPlace;
}

void MessageBuilder::add(EnumBnoAngle BNO_NAME, float value)
{
    int index = (int)BNO_NAME;
    bnoAngle[index].ID = BNO_NAME;
    bnoAngle[index].value = value;
}

void MessageBuilder::add(EnumBnoPosition BNO_NAME, float value)
{
    int index = (int)BNO_NAME;
    bnoPosition[index].ID = BNO_NAME;
    bnoPosition[index].value = value;
}

void MessageBuilder::add(EnumMotorPosition MOTOR_NAME, float value)
{
    int index = (int)MOTOR_NAME;
    motorPosition[index].ID = MOTOR_NAME;
    motorPosition[index].value = value;
}

void MessageBuilder::add(EnumIPType IP_NAME, IPAddress value)
{
    int index = (int)IP_NAME;
    ipAddress[index].ID = IP_NAME;
    ipAddress[index].value = value;
}

void MessageBuilder::buildMessage()
{
    clearMessage();
    // make a message using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    doc["log"] = logMessage;
    JsonArray bnoAngles = doc.createNestedArray("bnoAngles");
    for (int i = 0; i < NB_BNO_ANGLE; i++)
    {
        if (bnoAngle[i].ID != EnumBnoAngle::NONE)
        {
            JsonObject angle = bnoAngles.createNestedObject();
            angle["ID"] = (int)bnoAngle[i].ID;
            angle["value"] = bnoAngle[i].value;
        }
    }

    JsonArray bnoPositions = doc.createNestedArray("bnoPositions");
    for (int i = 0; i < NB_BNO_POSITION; i++)
    {
        if (bnoPosition[i].ID != EnumBnoPosition::NONE)
        {
            JsonObject bno_position = bnoPositions.createNestedObject();
            bno_position["ID"] = (int)bnoPosition[i].ID;
            bno_position["value"] = bnoPosition[i].value;
        }
    }

    JsonArray motorPositions = doc.createNestedArray("motorPositions");
    for (int i = 0; i < NB_MOTOR_POSITION; i++)
    {
        if (motorPosition[i].ID != EnumMotorPosition::NONE)
        {
            JsonObject motor_position = motorPositions.createNestedObject();
            motor_position["ID"] = (int)motorPosition[i].ID;
            motor_position["value"] = motorPosition[i].value;
        }
    }

}

void MessageBuilder::buildHandshake()
{
    clearMessage();
    // make a handshake using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    JsonArray ipAddresses = doc.createNestedArray("ipAddresses");
    for (int i = 0; i < NB_IP; i++)
    {
        if (ipAddress[i].ID != EnumIPType::NONE)
        {
            JsonObject ip_address = ipAddresses.createNestedObject();
            ip_address["ID"] = (int)ipAddress[i].ID;
            ip_address["value"] = ipAddress[i].value.toString();
        }
    }
    serializeJson(doc, message);
}

unsigned char* MessageBuilder::getMessage()
{
    return message;
}
