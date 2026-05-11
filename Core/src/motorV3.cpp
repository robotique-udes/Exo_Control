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

void MotorV3::packCmd(float position, float velocity, float kp, float kd, float torque){
    ///CAN Command Packet Structure (64 bits total, 8 bytes)///
    /// Bits  0-15:  Position command, range: -4*pi to 4*pi
    /// Bits 16-27:  Velocity command, range: -30 to +30 rad/s
    /// Bits 28-39:  Kp (Proportional gain), range: 0 to 500 N-m/rad
    /// Bits 40-51:  Kd (Derivative gain), range: 0 to 100 N-m*s/rad
    /// Bits 52-63:  Feed forward torque, range: -18 to 18 N-m

    ///limit data to be withing bounds///
    position = constrain(position, P_MIN, P_MAX); ///fminf(fmaxf(P_MIN, p_in(, P_MAX);
    velocity = constrain(velocity, V_MIN, V_MAX); ///fminf(fmaxf(V_MIN, v_in(, V_MAX);
    kp = constrain(kp, KP_MIN, KP_MAX); ///fminf(fmaxf(KP_MIN, kp_in(, KP_MAX);
    kd = constrain(kd, KD_MIN, KD_MAX); ///fminf(fmaxf(KD_MIN, kd_in(, KD_MAX);
    torque = constrain(torque, T_MIN, T_MAX); ///fminf(fmaxf(T_MIN, t_in(, V_MAX);


    ///convert floats to unsigned ints///
    unsigned int position_int = float_to_uint(position, P_MIN, P_MAX, 16);
    unsigned int velocity_int = float_to_uint(velocity, V_MIN, V_MAX, 12);
    unsigned int kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    unsigned int kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);
    unsigned int torque_int = float_to_uint(torque, T_MIN, T_MAX, 12);


    /// pack ints into the can buffer///
    byte buf[8];
    buf[0] = kp_int >> 8;
    buf[1] = ((kp_int & 0xF) <<4) | (kd_int >>8);
    buf[2] = kd_int >>4;
    buf[3] = position_int >> 8;
    buf[4] = position_int & 0xFF;
    buf[5] = velocity_int >> 4;
    buf[6] = ((velocity_int & 0xF) <<4) | (torque_int >>8);
    buf[7] = torque_int & 0xFF;
    for(int i = 0;i < 8; i++){
        msg.data[i] = buf[i];
    }
    msg.identifier = motorId;
    msg.extd = 1;
    msg.ss = 1;
    msg.data_length_code = 8;
}

void MotorV3::unpackReply(){
    
    //from cubemars doc, we only use the temperature for now, should aslo check for error 
    //TODO also evaluate motor errors 
    int16_t pos_int = (msg.data[0] << 8 | msg.data[1]);
    int16_t spd_int = (msg.data[2] << 8 | msg.data[3]);
    int16_t cur_int = (msg.data[4] << 8 | msg.data[5]);
    float motor_pos = (float)( pos_int * 0.1f); //Motor position
    float motor_spd = (float)( spd_int * 10.0f);//Motor speed
    float motor_cur = (float) ( cur_int * 0.01f);//Motor current
    temperature = msg.data[6] ;//Motor temperature
    int8_t motor_error = msg.data[7] ;//Motor error code
}




void MotorV3::sendCommand(MotorMode mode,float value){

    if (mode == TORQUE) {
        currentTorque = value;
        /// Correction of the torque value ///
        //value = ((value) / motorCorrectionSlope)*1000; // the command is in mA so we multiply by 1000
        packCmd(0,0,0,0,value);
        sendCanMessage(&msg);
    }
    receiveCanMessage(&msg);
    uint8_t source_id = msg.identifier;
    if (source_id == this->motorId) {
        unpackReply();
    }
} 

