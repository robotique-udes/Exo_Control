//TODO - implementer le code du V3
#include "motorV3.h"

MotorV3::MotorV3(){}

MotorV3::MotorV3(int id)
: Motor(id)
{
}

MotorV3::~MotorV3()
{
}

void MotorV3::sendRequest(MotorMode mode, float value)
{
    sendCommand(mode, value);
}

void MotorV3::sendCanMessage(CanFrame* msg){
  ESP32Can.writeFrame(msg);
}

void MotorV3::receiveCanMessage(CanFrame* msg){
  ESP32Can.readFrame(msg);
}

void MotorV3::packCmd(float torque){
    ///limit data to be withing bounds///
    torque = constrain(torque, T_MIN, T_MAX); ///fminf(fmaxf(T_MIN, t_in(, V_MAX);


    ///convert floats to unsigned ints///
    unsigned int torque_int = float_to_uint(torque, T_MIN, T_MAX, 12);


    /// pack ints into the can buffer///

    //!!! To confirm !!!//
    byte buf[4];
    buf[0] = torque_int >> 24;
    buf[1] = torque_int >> 16;
    buf[2] = torque_int >> 8;
    buf[3] = torque_int;

    for(int i = 0;i < 4; i++){
        msg.data[i] = buf[i];
    }
    msg.identifier = motorId;
    msg.extd = 1;
    msg.ss = 1;
    msg.data_length_code = 4;
}

void MotorV3::unpackReply(){
    
}

void MotorV3::sendCommand(MotorMode mode,float value){

  if (mode == TORQUE) {
    currentTorque = value;
    /// Correction of the torque value ///
    value = (value - motorCorrectionOffset) / motorCorrectionSlope;
    packCmd(value);
    sendCanMessage(&msg);
    receiveCanMessage(&msg);
    unpackReply();
  }
} 

unsigned int MotorV3::float_to_uint(float x, float x_min, float x_max, int bits){
    ///Converts a  float to an unsigned int, given range and number of bits///
    float span = x_max-x_min;
    float offset = x_min;
    unsigned int pgg = 0;
    if(bits==12){
        pgg = (unsigned int) ((x-offset)*4095.0/span);
    }
    if(bits==16){
        pgg = (unsigned int) ((x-offset)*65535.0/span);
    }
    return pgg;
}

float MotorV3::uint_to_float(unsigned int x_int, float x_min, float x_max, int bits){
    ///converts unsigned int to float, given range and number of bits///
    float span = x_max-x_min;
    float offset = x_min;
    float pgg = 0;
    if (bits==12){
        pgg = ((float) x_int)*span/4095 + offset;
    }
    if (bits==16){
        pgg = ((float) x_int)*span/65535.0 + offset;
    }
    return pgg;
}