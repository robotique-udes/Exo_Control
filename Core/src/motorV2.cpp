#include "motorV2.h"
//bool motorMode = ON; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test

MotorV2::MotorV2(){}

MotorV2::MotorV2(int id)
: Motor(id)
{
}


MotorV2::~MotorV2()
{
}

void MotorV2::sendRequest(MotorMode mode, float value)
{
    sendCommand(mode, value);
}

void MotorV2::start()
{
  Serial.print("Starting motor : ");
  Serial.println(this->motorId);
  enterMode();
  delay(100);
  zeroSet();
  delay(100);
}

void MotorV2::stop()
{
  exitMode();
}

void MotorV2::enterMode(){
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFC;
    msg.identifier = motorId;
    msg.extd = 0;
    msg.ss = 1;
    msg.data_length_code = 8;
    msg.dlc_non_comp = 0;
    msg.rtr = 0;
    msg.self = 0;
  sendCanMessage(&msg);
}

void MotorV2::exitMode(){
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFD;
    msg.identifier = motorId;
    msg.extd = 0;
    msg.ss = 1;
    msg.data_length_code = 8;
    msg.dlc_non_comp = 0;
    msg.rtr = 0;
    msg.self = 0;
    sendCanMessage(&msg);
}

void MotorV2::zeroSet(){
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFE;
    msg.identifier = motorId;
    msg.extd = 0;
    msg.ss = 1;
    msg.data_length_code = 8;
    msg.dlc_non_comp = 0;
    msg.rtr = 0;
    msg.self = 0;
    sendCanMessage(&msg);
    
}

void MotorV2::packCmd(float position, float velocity, float kp, float kd, float torque){
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
    buf[0] = position_int >> 8;
    buf[1] = position_int & 0xFF;
    buf[2] = velocity_int >> 4;
    buf[3] = ((velocity_int & 0xF) <<4) | (kp_int >>8);
    buf[4] = kp_int & 0xFF;
    buf[5] = kd_int >>4;
    buf[6] = ((kd_int & 0xF) <<4) | (torque_int >>8);
    buf[7] = torque_int & 0xFF;
    for(int i = 0;i < 8; i++){
        msg.data[i] = buf[i];
    }
    msg.identifier = motorId;
    msg.extd = 0;
    msg.ss = 1;
    msg.data_length_code = 8;
    msg.dlc_non_comp = 0;
    msg.rtr = 0;
    msg.self = 0;
}
// 
void MotorV2::unpackReply(CanFrame msgReply){
    unsigned int id = msgReply.data[0];
    unsigned int position_int = (msgReply.data[1] << 8) | msgReply.data[2];
    unsigned int velocity_int = (msgReply.data[3] << 4) | (msgReply.data[4] >> 4);
    unsigned int current_int = ((msgReply.data[4] & 0xF) << 8) | msgReply.data[5];
    temperature = msgReply.data[6]-40;
}

//
void MotorV2::sendCommand(MotorMode mode,float value){
  switch (mode)
  {
  case TORQUE:
    currentTorque = value;
    currentTorque = value;
    /// Correction of the torque value ///
    //value = (value - motorCorrectionOffset) / motorCorrectionSlope;

    packCmd(0, 0, 0, 0, value);
    sendCanMessage(&msg);
    break;
  
  case VELOCITY:
    packCmd(0, value, KP, KD, 0);
    sendCanMessage(&msg);
    //receiveCanMessage(&msg);
    //unpackReply();
    break;
  case POSITION:
    packCmd(value, 0, KP, KD, 0);
    sendCanMessage(&msg);
    //receiveCanMessage(&msg);
    //unpackReply();
  default:
    break;
  }

/*   int attempts = 0;
  while (attempts < 10) {

    //receiveCanMessage(&msg);
    if (msg.data[0] == this->motorId) {
        //unpackReply();
        return;
    } 
    attempts++;
  } */
} 

