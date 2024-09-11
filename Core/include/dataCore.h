#ifndef DATACORE_H
#define DATACORE_H

#include <Arduino.h>
#include <Nextion.h>

#include "define.h"
#include "enums.h"
#include "relay.h"
#include "structs.h"

using namespace std;

class DataCore
{
private:
    Relay relais;

    // Ins
    // proxim
    bool rightProxiState;
    bool leftProxiState;
    bool proximEnabled;
    bool needResetProxim;

    // encoder
    float encoder_knee_right = 0;
    float encoder_knee_left = 0;
    float encoder_hip_right = 0;
    float encoder_hip_left = 0;
    bool needResetEncoder = false;

    // BNO
    float Imu_knee_right = 0;
    float Imu_knee_left = 0;
    float Imu_hip_right = 0;
    float Imu_hip_left = 0;
    float Imu_back = 0;
    float Imu_thigh_left = 0;
    float Imu_thigh_right = 0;
    float Imu_tibia_left = 0;
    float Imu_tibia_right = 0;

    array<BNOStruct *, 5> bnoData;

    //In between settings
    bool motorEnabled;
    bool clutchEnabled;
    bool sonarState;
    bool angleSource;
    bool brightness;
    int height;
    float motorPower;

    // Outs
    // PWM
    int PWMRightKnee = 0;
    int PWMLeftKnee = 0;
    int PWMRightHip = 0;
    int PWMLeftHip = 0;
    
    DataCore();

public:


    /**
    * @brief Clutch enable getter
    * @return Clutch state
    */
    bool isClutchEnabled();

    /**
    * @brief Clutch enable setter
    * @param setClutchEnabled Clutch enable state
    */
    void setClutchEnabled(bool setClutchEnabled);

    

    /**
    * @brief Motor enable setter
    * @param setMotorEnabled motor enable state
    */
    void setMotorEnabled(bool setMotorEnabled);

    /**
    * @brief Motor enable getter
    */
    bool isMotorEnabled();

    /**
    * @brief Proxim enable setter
    * @param setProximEnabled Proxim enable state
    */
    void setProximEnabled(bool setProximEnabled);

    /**
    * @brief Proxim enable getter
    * @return Proxim state
    */
    bool isProximEnabled();
    
    /**
    * @brief Put all settings back to boot values
    */
    void initialise();

    /**
    * @brief Set all encoder pulse values to 0
    */
    void resetEncoder();

    /**
    * @brief Toggle brightness mode for proxims
    */
    void setBrightness();
    /**
    * @brief Brightness getter
    * @return Brightness state
    */
    bool getBrightness();

    /**
    * @brief Proxim reset getter
    * @return Proxim reset state
    */
    bool getResetProxim();

    /**
    * @brief Proxim reset setter
    * @param reset Proxim reset state
    */
    void setResetProxim(bool reset);

    /**
    * @brief Angle source getter
    * @return Angle source state
    */
    bool getAngleSource();

    /**
    * @brief Angle source setter
    * @param setAngleSource Angle source state
    */
    void setAngleSource(bool setAngleSource);

    /**
    * @brief Motor power getter
    * @return Motor power value state
    */
    int getMotorPower();

    /**
    * @brief Motor power setter
    * @param setMotorPower Motor power value
    */
    void setMotorPower(int setMotorPower);

    /**
    * @brief Change motor power using an offset
    * @param setMotorPower Motor power offset value
    */
    void adjustMotorPower(int powerOffset);

    // proxim
    /**
    * @brief Right proxi getter
    * @return Right proxi state (1 if grounded, 0 if not)
    */
    bool getRightProxi();

    /**
    * @brief Left proxi getter
    * @return Left proxi state (1 if grounded, 0 if not)
    */
    bool getLeftProxi();

    /**
    * @brief Right proxi state setter
    * @param state Current state (1 if grounded, 0 if not)
    */
    void setRightProxi(bool state);

    /**
    * @brief left proxi state setter
    * @param state Current state (1 if grounded, 0 if not)
    */
    void setLeftProxi(bool state);

    // encoder
    /**
    * @brief Encoder getter (Deg)
    * @param motor Target motor
    * @return Degree angle value
    */
    float getEncoderDeg(EnumMotorPosition motor);

    /**
    * @brief Encoder getter (Rad)
    * @param motor Traget motor
    * @return Radian angle value
    */
    float getEncoderRad(EnumMotorPosition motor);

    /**
    * @brief Encoder angle setter
    * @param motor Target motor
    * @param pulse Pulse value
    */
    void setEncoderAngles(EnumMotorPosition motor, int pulse);

    /**
    * @brief Encoder reset getter
    * @return Encoder reset state
    */
    bool isEncoderResetNeeded();

    /**
    * @brief Encoder reset setter
    * @param state New state
    */
    void setEncoderReset(bool state);

    // Bno
    /**
    * @brief Bno angle setter
    * @param bno Target bno
    * @param angle Angle value
    */
    void setBnoAngles(EnumBnoAngle bno, float angle);

    /**
    * @brief Bno angle getter
    * @param bno Target bno
    * @return Degree angle value
    */
    float getBnoAngles(EnumBnoAngle bno);

    /**
    * @brief Bno struct setter :)
    * @param bno BNO title
    * @param data Pointer to the structure
    */
    void setBnoStruct(EnumBnoPosition bno, BNOStruct* data);

    /**
    * @brief Bno struct getter :)
    * @param bno Target bno
    * @return struct
    */
    BNOStruct* getBnoStruct(EnumBnoPosition bno);

    // PWM
    /**
    * @brief PWM setter
    * @param motor Target motor
    * @param pwm PWM value
    */
    void setPWM(EnumMotorPosition motor, float pwm);

    /**
    * @brief PWM getter
    * @param motor Target motor
    * @return PWM value
    */
    float getPWM(EnumMotorPosition motor);

    // Singleton code
    static DataCore *instance;
    /**
     * @brief Create new instance if none existe, return existing singleton otherwise
     * @return Singletan instance
     */
    static DataCore &getInstance();
    DataCore(const DataCore &) = delete;
    DataCore &operator=(const DataCore &) = delete;
};

#endif