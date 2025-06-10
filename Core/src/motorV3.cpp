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

void MotorV3::packCmd(float torque){
    ///limit data to be withing bounds///
    torque = constrain(torque, T_MIN, T_MAX); ///fminf(fmaxf(T_MIN, t_in(, V_MAX);


    ///convert floats to unsigned ints///
    unsigned int torque_int = float_to_uint(torque, T_MIN, T_MAX, 16);


    /// pack ints into the can buffer///

    //!!! To confirm !!!//
    byte buf[4];
    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = (torque_int >> 8) & 0xFF;
    buf[3] = torque_int & 0xFF;

    for(int i = 0;i < 4; i++){
        msg.data[i] = buf[i];
    }
    msg.identifier = motorId;
    msg.extd = 1;
    msg.ss = 1;
    msg.data_length_code = 4;
}

void MotorV3::unpackReply(){
    // To do 
}

void MotorV3::sendCommand(MotorMode mode,float value){

  if (mode == TORQUE) {
    currentTorque = value;
    /// Correction of the torque value ///
    value = (value - motorCorrectionOffset) / motorCorrectionSlope * 1000; // the command is in mA so we multiply by 1000
    packCmd(value);
    sendCanMessage(&msg);
    //receiveCanMessage(&msg);
    //unpackReply();
  }
} 

