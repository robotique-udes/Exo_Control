#ifndef MessageBuiler_h
#define MessageBuiler_h

#define NB_DE_MESSAGE 1
#include "enums.h"

struct values
{
    int ID;
    float value;
};

class MessageBuilder
{
    private:
        unsigned char message[255 * NB_DE_MESSAGE];
        values bnoAngle[9];
        values bnoPosition[5];
        values motorPosition[4];
        
    public:
        MessageBuilder();
        unsigned char* getMessage();
        void clearMessage();
        void add(unsigned char log[]);
        void add(EnumBnoAngle angle, float value);
        void add(EnumBnoPosition position, float value);
        void add(EnumMotorPosition motor, float value);
        void genrateMessage();

};

#endif