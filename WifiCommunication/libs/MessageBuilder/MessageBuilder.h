#ifndef MessageBuiler_h
#define MessageBuiler_h
#include "enums.h"
#include <WiFi.h>
#include <ArduinoJson.h>

#define NESTED_LOGS "logs"
#define NESTED_BNO_ANGLES "bnoAngles"
#define NESTED_BNO_POSITIONS "bnoPositions"
#define NESTED_BNO_MOTEUR "motorPositions"
#define NESTED_IP_TYPE "IP_Type"

#define NB_LOGS 10
#define NB_BNO_ANGLE 9
#define NB_BNO_POSITION 5
#define NB_MOTOR_POSITION 4
#define NB_IP 10
#define MESSAGE_LENGTH 512
#define LOG_LENGTH 128


struct structBnoAngles
{
    EnumBnoAngle ID;
    float value;
};

struct structBnoPosition
{
    EnumBnoPosition ID;
    float value;
};

struct structMotorPosition
{
    EnumMotorPosition ID;
    float value;
};

struct structIPAddress
{
    EnumIPType ID;
    String value;
    uint32_t ipAdd32;
};

class MessageBuilder
{
    private:
        unsigned char message[MESSAGE_LENGTH];
        unsigned char logMessage[NB_LOGS][LOG_LENGTH];
        structBnoAngles bnoAngle[NB_BNO_ANGLE];
        structBnoPosition bnoPosition[NB_BNO_POSITION];
        structMotorPosition motorPosition[NB_MOTOR_POSITION];
        structIPAddress ipAddress[NB_IP];
        int indexStructBnoAngles;
        int indexStructBnoPosition;
        int indexStructMotorPosition;
        int indexStructIPAddressTest;

        int logPlace = 127;
        int lengthMessage;
        int nbrLogs = 0;

        String castUint32ToStringIP(uint32_t val);
        
    public:
        MessageBuilder();
        unsigned char* getMessage();
        void clearMessage();
        void clearInfo();
        void add(unsigned char log[LOG_LENGTH]);
        int getLogPlace();
        int getMessageLength();
        void add(EnumBnoAngle BNO_NAME, float value);
        void add(EnumBnoPosition BNO_NAME, float value);
        void add(EnumMotorPosition MOTOR_NAME, float value);
        void add(EnumIPType IP_NAME, IPAddress *value);
        int buildMessage();
        int buildHandshake();
};
#endif