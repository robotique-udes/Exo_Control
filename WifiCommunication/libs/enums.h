#ifndef ENUMS_H
#define ENUMS_H
enum class EnumAngleSource
{
    IMU,
    ENCODER,
};

enum class EnumBnoPosition
{
    THIGH_L    =   0,
    THIGH_R    =   1,
    TIBIA_L    =   2,
    TIBIA_R    =   3,
    EXO_BACK   =   4,
    NONE       =   5
};

enum class EnumBnoAngle
{
    THIGH_L    =   0,
    THIGH_R    =   1,
    TIBIA_L    =   2,
    TIBIA_R    =   3,
    EXO_BACK   =   4,
    HIP_L      =   5,
    HIP_R      =   6,
    KNEE_L     =   7,
    KNEE_R     =   8,
    NONE       =   9
};

enum class EnumSonarState
{
    SQUAT,
    WALK,
};

enum class EnumMotorPosition
{
    HIP_R,
    HIP_L,
    KNEE_R,
    KNEE_L,
    NONE
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

enum class IPType
{
    WATCH,
    EXOSKELETON,
    SIMULATION,
    CLIENT_3,
    CLIENT_4,
    CLIENT_5,
    CLIENT_6,
    CLIENT_7,
    CLIENT_8,
    CLIENT_9,
    NONE
};

#endif