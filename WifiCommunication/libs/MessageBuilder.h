#ifndef MessageBuiler_h
#define MessageBuiler_h

#include "enums.h"

#define NB_MESSAGE 1
#define NB_BNO_ANGLE 9
#define NB_BNO_POSITION 5
#define NB_MOTOR_POSITION 4

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
        unsigned char message[255 * NB_MESSAGE];
        valuesBnoAngles bnoAngle[NB_BNO_ANGLE];
        valuesBnoPosition bnoPosition[NB_BNO_POSITION];
        valuesMotorPosition motorPosition[NB_MOTOR_POSITION];
        
    public:
        MessageBuilder();
        unsigned char* getMessage();
        void clearMessage();
        void clearInfo();
        void add(unsigned char log[]);
        void add(EnumBnoAngle angle, float value);
        void add(EnumBnoPosition position, float value);
        void add(EnumMotorPosition motor, float value);
        void buildMessage();

};

#endif