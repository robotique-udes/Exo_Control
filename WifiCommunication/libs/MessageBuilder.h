#ifndef MessageBuiler_h
#define MessageBuiler_h

#include "enums.h"

#define NB_MESSAGE 1
#define NB_BNO_ANGLE 9
#define NB_BNO_POSITION 5
#define NB_MOTOR_POSITION 4
#define MESSAGE_LENGTH 512
#define LOG_LENGTH 128

struct valuesBnoAngles
{
    EnumBnoAngle ID;
    float value;
};

struct valuesBnoPosition
{
    EnumBnoPosition ID;
    float value;
};

struct valuesMotorPosition
{
    EnumMotorPosition ID;
    float value;
};

class MessageBuilder
{
    private:
        unsigned char message[MESSAGE_LENGTH];
        unsigned char logMessage[LOG_LENGTH];
        valuesBnoAngles bnoAngle[NB_BNO_ANGLE];
        valuesBnoPosition bnoPosition[NB_BNO_POSITION];
        valuesMotorPosition motorPosition[NB_MOTOR_POSITION];
        int logPlace = 0;
        
    public:
        MessageBuilder();
        unsigned char* getMessage();
        void clearMessage();
        void clearInfo();
        void add(unsigned char log[LOG_LENGTH]);
        int getLogPlace();
        void add(EnumBnoAngle BNO, float value);
        void add(EnumBnoPosition BNO, float value);
        void add(EnumMotorPosition MOTOR, float value);
        void buildMessage();

};

#endif