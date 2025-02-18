
#include "MessageBuilder.h"

#define ENUM_BNO_ANGLE "EnumBnoAngle"
#define ENUM_BNO_POSITION "EnumBnoPosition"
#define ENUM_MOTOR_POSITION "EnumMotorPosition"
#define ENUM_IP_TYPE "EnumIpType"

MessageBuilder::MessageBuilder()
{
    lengthMessage = 0;
    
    indexStructIPAddressTest = 0;
    for(int i = 0; i < NB_IP; i++)
    {
        ipAddress[i].ID = EnumIPType::NONE;
        ipAddress[i].value = "0.0.0.0";
    }
    clearInfo();
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
    for (int i = 0; i < NB_LOGS; i++)
    {
        for (int j = 0; j < LOG_LENGTH; j++)
        {
            logMessage[i][j] = 0;
        }
    }

    indexStructBnoAngles = 0;
    indexStructBnoPosition = 0;
    indexStructMotorPosition = 0;
    indexStructIPAddressTest = 0;
}

void MessageBuilder::add(unsigned char *log)
{   
    if (nbrLogs >= NB_LOGS)
        return;

    for (int i = 0; i < LOG_LENGTH; i++)
    {
        logMessage[nbrLogs][i] = log[i];
    }
    nbrLogs++;
}

int MessageBuilder::getLogPlace()
{
    return logPlace;
}

int MessageBuilder::getMessageLength()
{
    return lengthMessage;
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
    Serial.print(indexStructIPAddressTest);
    Serial.print("  Adding Ip address");
    Serial.print((char)IP_NAME);
    Serial.print("     ");
    Serial.println(value->toString());
    ipAddress[indexStructIPAddressTest].ID = IP_NAME;
    ipAddress[indexStructIPAddressTest].value = value->toString();
    indexStructIPAddressTest++;  
}

int MessageBuilder::buildMessage()
{
    clearMessage();
    // make a message using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);

    JsonArray logs = doc.createNestedArray(NESTED_LOGS);
    for (int i = 0; i < nbrLogs; i++)
    {
        JsonObject log = logs.createNestedObject();
        log["ID"] = i;
        log["value"] = String((char*)logMessage[i]);  
    }

    JsonArray bnoAngles = doc.createNestedArray(NESTED_BNO_ANGLES);
    Serial.println("bnoAngles NestedArray");
    for (int i = 0; i < indexStructBnoAngles; i++)
    {
        if (bnoAngle[i].ID != EnumBnoAngle::NONE)
        {
            JsonObject angle = bnoAngles.createNestedObject();
            angle["ID"] = (int)bnoAngle[i].ID;
            angle["value"] = bnoAngle[i].value;
        }
    }

    JsonArray bnoPositions = doc.createNestedArray(NESTED_BNO_POSITIONS);
    Serial.println("bnoPositions NestedArray");
    Serial.print(indexStructBnoPosition);
    for (int i = 0; i < indexStructBnoPosition; i++)
    {
        Serial.print(i);
        if (bnoPosition[i].ID != EnumBnoPosition::NONE)
        {
            JsonObject bno_position = bnoPositions.createNestedObject();
            bno_position["ID"] = (int)bnoPosition[i].ID;
            bno_position["value"] = bnoPosition[i].value;
        }
    }
    JsonArray motorPositions = doc.createNestedArray(NESTED_BNO_MOTEUR);
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
    serializeJson(doc, Serial);
    return lengthMessage;
}

int MessageBuilder::buildHandshake()
{
    clearMessage();
    // make a handshake using json
    DynamicJsonDocument doc(MESSAGE_LENGTH);
    JsonArray ipAddresses = doc.createNestedArray(ENUM_IP_TYPE);
    for (int i = 0; i < NB_IP; i++)
    {
        if (ipAddress[i].ID != EnumIPType::NONE)
        {
            JsonObject ip_address = ipAddresses.createNestedObject();
            ip_address["ID"] = (int)ipAddress[i].ID;
            ip_address["value"] = ipAddress[i].value;
        }
    }

    serializeJson(doc, Serial);
    return lengthMessage = serializeJson(doc, message);
}

unsigned char* MessageBuilder::getMessage()
{
    return message;
}

String MessageBuilder::castUint32ToStringIP(uint32_t val) {
    return String(val & 0xFF) + "." +
           String((val >> 8) & 0xFF) + "." +
           String((val >> 16) & 0xFF) + "." +
           String((val >> 24) & 0xFF);
}