#ifndef ENUMS_H
#define ENUMS_H
enum class EnumAngleSource
{
    IMU,
    ENCODER,
};

enum class enumIMU
{
    HIP_L    =   0,
    KNEE_L   =   1,
    HIP_R    =   2,
    KNEE_R   =   3,
    EXO_BACK =   4    
};

enum class EnumSonarState
{
    SQUAT,
    WALK,
};

enum class EnumScreenCallback
{
    BUTTON_TOGGLE_MOTOR,
    BUTTON_TOGGLE_CLUTCH,
    BUTTON_INIT,
    BUTTON_RESET_ENCODER,
    BUTTON_AUTO_CALIB_PROXIM,
    BUTTON_TOGGLE_ANGLE_SOURCE,
    BUTTON_TOGGLE_PROXIM,
    BUTTON_TEST,
    BUTTON_DEV_1,
    BUTTON_DEV_2,
    BUTTON_DEV_TOGGLE_1,
    BUTTON_DEV_TOGGLE_2,
    SLIDER_DEV_1,
    SLIDER_MOTOR_POWER,
};

#endif