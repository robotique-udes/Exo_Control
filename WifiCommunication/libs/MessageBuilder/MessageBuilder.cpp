
#include "MessageBuilder.h"


MessageBuilder::MessageBuilder()
{
    lengthMessage = 0;

    IPAddress value = IPAddress();
    
    indexStructIPAddressTest = 0;
    for(int i = 0; i < NB_IP; i++)
    {
        ipAddress[i].ID = EnumIPType::NONE;
        ipAddress[i].value = value;
    }
}

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

    indexStructBnoAngles = 0;
    indexStructBnoPosition = 0;
    indexStructMotorPosition = 0;
    indexStructIPAddressTest = 0;
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
    bnoAngle[indexStructBnoAngles].ID = BNO_NAME;
    bnoAngle[indexStructBnoAngles].value = value;
    indexStructBnoAngles++;
}

void MessageBuilder::add(EnumBnoPosition BNO_NAME, float value)
{
    bnoPosition[indexStructBnoPosition].ID = BNO_NAME;
    bnoPosition[indexStructBnoPosition].value = value;
    indexStructBnoPosition++;
}

void MessageBuilder::add(EnumMotorPosition MOTOR_NAME, float value)
{
    motorPosition[indexStructMotorPosition].ID = MOTOR_NAME;
    motorPosition[indexStructMotorPosition].value = value;
    indexStructMotorPosition++;
}

void MessageBuilder::add(EnumIPType IP_NAME, IPAddress* value)
{
    ipAddress[indexStructIPAddressTest].ID = IP_NAME;
    ipAddress[indexStructIPAddressTest].value = *value;
    indexStructIPAddressTest++;  
}

int MessageBuilder::buildMessage()
{
    clearMessage();
    // make a message using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    doc["log"] = logMessage;
    JsonArray bnoAngles = doc.createNestedArray("bnoAngles");
    for (int i = 0; i < indexStructBnoAngles; i++)
    {
        if (bnoAngle[i].ID != EnumBnoAngle::NONE)
        {
            JsonObject angle = bnoAngles.createNestedObject();
            angle["ID"] = (int)bnoAngle[i].ID;
            angle["value"] = bnoAngle[i].value;
        }
    }
    JsonArray bnoPositions = doc.createNestedArray("bnoPositions");
    for (int i = 0; i < indexStructBnoPosition; i++)
    {
        if (bnoPosition[i].ID != EnumBnoPosition::NONE)
        {
            JsonObject bno_position = bnoPositions.createNestedObject();
            bno_position["ID"] = (int)bnoPosition[i].ID;
            bno_position["value"] = bnoPosition[i].value;
        }
    }
    JsonArray motorPositions = doc.createNestedArray("motorPositions");
    for (int i = 0; i < indexStructMotorPosition; i++)
    {
        if (motorPosition[i].ID != EnumMotorPosition::NONE)
        {
            JsonObject motor_position = motorPositions.createNestedObject();
            motor_position["ID"] = (int)motorPosition[i].ID;
            motor_position["value"] = motorPosition[i].value;
        }
    }
    lengthMessage = serializeJson(doc, message);
    return lengthMessage;
}

int MessageBuilder::buildHandshake()
{
    clearMessage();
    // make a handshake using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    JsonArray ipAddresses = doc.createNestedArray("ipAddresses");
    for (int i = 0; i < NB_IP; i++)
    {
        Serial.print("ipAddress[i].ID: ");
        Serial.println((int)ipAddress[i].ID);
        if (ipAddress[i].ID != EnumIPType::NONE)
        {
            JsonObject ip_address = ipAddresses.createNestedObject();
            ip_address["ID"] = (int)ipAddress[i].ID;
            String cal =  castUint32ToStringIP(ipAddress[i].ipAdd32);
            //ip_address["value"] = castUint32ToStringIP(ipAddress[i].ipAdd32);
            ip_address["value"] = ipAddress[i].value.toString();
        }
    }

    return lengthMessage = serializeJson(doc, message);
}

unsigned char* MessageBuilder::getMessage()
{
    return message;
}

void MessageBuilder::add(EnumIPType IP_NAME, uint32_t address)
{
    ipAddress[indexStructIPAddressTest].ID = IP_NAME;
    ipAddress[indexStructIPAddressTest].ipAdd32 = address;
    indexStructIPAddressTest++;    
}

String MessageBuilder::castUint32ToStringIP(uint32_t val) {
    return String(val & 0xFF) + "." +
           String((val >> 8) & 0xFF) + "." +
           String((val >> 16) & 0xFF) + "." +
           String((val >> 24) & 0xFF);
}