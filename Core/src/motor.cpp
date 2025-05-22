#include "motor.h"
#include "enums.h"
bool motorMode = ON; // mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test

Motor::Motor()
{
}


Motor::~Motor()
{
}

void Motor::setPins()
{
  pinMode(PIN_MD_HAN_GAU_CT, INPUT);
  pinMode(PIN_MD_HAN_DRO_CT, INPUT);
  pinMode(PIN_MD_GEN_GAU_CT, INPUT);
  pinMode(PIN_MD_GEN_DRO_CT, INPUT);

  pinExtender.pinMode(PIN_MD_GEN_GAU_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_GEN_DRO_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_HAN_GAU_EN, OUTPUT);
  pinExtender.pinMode(PIN_MD_HAN_DRO_EN, OUTPUT);
}


void Motor::sendCanMessage(CanFrame* msg){
  ESP32Can.writeFrame(msg);
}

void Motor::receiveCanMessage(CanFrame* msg){
  ESP32Can.readFrame(msg);
}

void Motor::enterMode(){
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
  sendCanMessage(&msg);
}

void Motor::exitMode(){
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
    sendCanMessage(&msg);
}

void Motor::zeroSet(){
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
    sendCanMessage(&msg);
    
}

void Motor::packCmd(float position, float velocity, float kp, float kd, float torque){
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
}
// 
void Motor::unpackReply(){
    unsigned int id = msg.data[0];
    unsigned int position_int = (msg.data[1] << 8) | msg.data[2];
    unsigned int velocity_int = (msg.data[3] << 4) | (msg.data[4] >> 4);
    unsigned int current_int = ((msg.data[4] & 0xF) << 8) | msg.data[5];
    temperature = msg.data[6]-40;
}

//
void Motor::sendCommand(MotorMode mode,float value){
  switch (mode)
  {
  case TORQUE:
    currentTorque = value;
    packCmd(0, 0, 0, 0, value);
    sendCanMessage(&msg);
    receiveCanMessage(&msg);
    unpackReply();
    break;
  
  case VELOCITY:
    packCmd(0, value, KP, KD, 0);
    sendCanMessage(&msg);
    receiveCanMessage(&msg);
    unpackReply();
    break;
  case POSITION:
    packCmd(value, 0, KP, KD, 0);
    sendCanMessage(&msg);
    receiveCanMessage(&msg);
    unpackReply();
  default:
    break;
  }
} 

//
int Motor::getTemperature(){
  return temperature;
}

//
void Motor::setMotorId(int id){
  motorId = id;
}

//
int Motor::getMotorId(){
  return motorId;
}

unsigned int Motor::float_to_uint(float x, float x_min, float x_max, int bits){
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

float Motor::uint_to_float(unsigned int x_int, float x_min, float x_max, int bits){
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

enum EnumMotorPosition Motor::getMotorPosition()
{
  return motorPosition;
}

float Motor::getCurrentTorque()
{
  return currentTorque;
}