
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

void MessageBuilder::add(EnumBnoAngle BNO, float value)
{
    int index = (int)BNO;
    bnoAngle[index].ID = BNO;
    bnoAngle[index].value = value;
}

void MessageBuilder::add(EnumBnoPosition BNO, float value)
{
    int index = (int)BNO;
    bnoPosition[index].ID = BNO;
    bnoPosition[index].value = value;
}

void MessageBuilder::add(EnumMotorPosition MOTOR, float value)
{
    int index = (int)MOTOR;
    motorPosition[index].ID = MOTOR;
    motorPosition[index].value = value;
}

void MessageBuilder::buildMessage()
{
   // todo
}

unsigned char* MessageBuilder::getMessage()
{
    return message;
}
