
#include "messageBuilder.h"

void MessageBuilder::clearMessage()
{
    for (int i = 0; i < 255 * NB_DE_MESSAGE; i++)
    {
        message[i] = 0;
    }
}

void MessageBuilder::add(unsigned char log[])
{
    for (int i = 0; i < 255 * NB_DE_MESSAGE; i++)
    {
        if (message[i] == 0)
        {
            message[i] = log[i];
            break;
        }
    }
}

void MessageBuilder::add(EnumBnoAngle angle, float value)
{
    for (int i = 0; i < 9; i++)
    {
        if (bnoAngle[i].ID == 0)
        {
            bnoAngle[i].ID = (int)angle;
            bnoAngle[i].value = value;
            break;
        }
    }
}

void MessageBuilder::add(EnumBnoPosition position, float value)
{
    for (int i = 0; i < 5; i++)
    {
        if (bnoPosition[i].ID == 0)
        {
            bnoPosition[i].ID = (int)position;
            bnoPosition[i].value = value;
            break;
        }
    }
}

void MessageBuilder::add(EnumMotorPosition motor, float value)
{
    for (int i = 0; i < 4; i++)
    {
        if (motorPosition[i].ID == 0)
        {
            motorPosition[i].ID = (int)motor;
            motorPosition[i].value = value;
            break;
        }
    }
}

void MessageBuilder::genrateMessage()
{
    clearMessage();
    for (int i = 0; i < 9; i++)
    {
        if (bnoAngle[i].ID != 0)
        {
            message[bnoAngle[i].ID] = bnoAngle[i].value;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (bnoPosition[i].ID != 0)
        {
            message[bnoPosition[i].ID] = bnoPosition[i].value;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (motorPosition[i].ID != 0)
        {
            message[motorPosition[i].ID] = motorPosition[i].value;
        }
    }
}

unsigned char* MessageBuilder::getMessage()
{
    return message;
}
