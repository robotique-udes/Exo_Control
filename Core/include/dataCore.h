#ifndef DATACORE_H
#define DATACORE_H

#include <Arduino.h>
#include <Nextion.h>

#include "define.h"
#include "enums.h"

using namespace std;

typedef struct {
    // Quaternion components (real, i, j, k)
    float real;
    float i;
    float j;
    float k;
    // Linear acceleration
    float lin_x;
    float lin_y;
    float lin_z;
    // Accelerometer
    float accel_x;
    float accel_y;
    float accel_z;
} BnoData_t;

class DataCore
{
private:

    // Ins
    // ground contact detection
    bool rightGrounded;
    bool leftGrounded;
    bool groundDetectEnable;

    // BNO
    float Imu_back = 0;
    float Imu_thigh_left = 0;
    float Imu_thigh_right = 0;
    float Imu_tibia_left = 0;
    float Imu_tibia_right = 0;

    array<BnoData_t *, 5> bnoData;

    //In between settings
    bool motorEnabled;
    bool angleSource;
    float motorPower;

    // Outs
    // PWM
    int torqueRightKnee = 0;
    int torqueLeftKnee = 0;
    int torqueRightHip = 0;
    int torqueLeftHip = 0;
    
    DataCore();

public:

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
    * @brief Ground detection setter
    * @param setGroundDetectEnable Detection enable state
    */
    void setGroundDetectEnable(bool setGroundDetectEnable);

    /**
    * @brief Ground detection getter
    * @return Ground detection state
    */
    bool isGroundDetectEnable();
    
    /**
    * @brief Put all settings back to boot values
    */
    void initialise();

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

    /**
    * @brief Right proxi getter
    * @return Right proxi state (1 if grounded, 0 if not)
    */
    bool getRightGrounded();

    /**
    * @brief Left proxi getter
    * @return Left proxi state (1 if grounded, 0 if not)
    */
    bool getLeftGrounded();

    /**
    * @brief Right proxi state setter
    * @param state Current state (1 if grounded, 0 if not)
    */
    void setRightGrounded(bool state);

    /**
    * @brief left proxi state setter
    * @param state Current state (1 if grounded, 0 if not)
    */
    void setLeftGrounded(bool state);

    // Bno
    /**
    * @brief Bno angle setter
    * @param bno Target bno
    * @param angle Angle value
    */
    void setBnoAngle(EnumBnoPosition bno, float angle);

    /**
    * @brief Bno angle getter
    * @param bno Target bno
    * @return Degree angle value
    */
    float getBnoAngle(EnumBnoPosition bno);

    /**
    * @brief Bno struct setter :)
    * @param bno BNO title
    * @param data Pointer to the structure
    */
    void setBnoStruct(EnumBnoPosition bno, BnoData_t* data);

    /**
    * @brief Bno struct getter :)
    * @param bno Target bno
    * @return struct
    */
    BnoData_t* getBnoStruct(EnumBnoPosition bno);

    // PWM
    /**
    * @brief PWM setter
    * @param motor Target motor
    * @param pwm PWM value
    */
    void setTorque(EnumMotorPosition motor, float torque);

    /**
    * @brief PWM getter
    * @param motor Target motor
    * @return PWM value
    */
    float getTorque(EnumMotorPosition motor);

    // Singleton code
    static DataCore *instance;
    /**
     * @brief Create new instance if none existe, return existing singleton otherwise
     * @return Singletan instance
     */
    static DataCore &getInstance();
    DataCore(const DataCore &) = delete;
    DataCore &operator=(const DataCore &) = delete;

    void printAngles();
};

#endif