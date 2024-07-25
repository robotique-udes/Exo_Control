#include "logic.h"

void Logic::Update(){
    neededTorque();
    PIDCurrentPrealable();
}


template <typename T> void Logic::LimitMinMax(T &val, T cap)
{
    if (std::is_same<T, int>::val || std::is_same<T, float>::val)
        val > cap ? val = cap : val < -cap ? val = -cap : 0;
    else
        Serial.print("LimitMinMax Error - Invalide data type: ");
        Serial.println(val);
    // uncomment if pussy
    //  if (val > max)
    //  val = max;
    // else if (val < -max)
    // val = -max;
}

void Logic::neededTorque()
{
    float LeftHipAngle = 0;
    float LeftKneeAngle = 0;
    float RightHipAngle = 0;
    float RightKneeAngle = 0;

    if(dataCore.getAngleSource()==FROM_ENCODER){
        LeftHipAngle = dataCore.getEncoderDeg(EnumMotorPosition::HIP_L);
        LeftKneeAngle = dataCore.getEncoderDeg(EnumMotorPosition::KNEE_L);
        RightHipAngle = dataCore.getEncoderDeg(EnumMotorPosition::HIP_R);
        RightKneeAngle = dataCore.getEncoderDeg(EnumMotorPosition::KNEE_L);
    }
    else{
        LeftHipAngle = dataCore.getBnoAngles(EnumBnoPosition::HIP_L);
        LeftKneeAngle = dataCore.getBnoAngles(EnumBnoPosition::KNEE_L);
        RightHipAngle = dataCore.getBnoAngles(EnumBnoPosition::HIP_R);
        RightKneeAngle = dataCore.getBnoAngles(EnumBnoPosition::KNEE_L);
    }

    float LeftHipRAD = toRadian(LeftHipAngle);
    float LeftKneeRAD = toRadian(LeftKneeAngle);
    float RightHipRAD = toRadian(RightHipAngle);
    float RightKneeRAD = toRadian(RightKneeAngle);

    // If clutch are on automatic, calculate torque needed
    if (dataCore.isMotorEnabled())
    {
        // Right Hip Torque Equation
        if (!dataCore.getRightProxi())
            RightHipTorque = (MF * G) * sin(RightHipRAD) * LF / 2 + (MT * G) * (sin(RightHipRAD) * LF + sin(RightKneeRAD - RightHipRAD) * LT / 2); // TODO : TEST THIS NEW EQUATION
        else
        {
            if (RightHipAngle < 110)
            {
                RightHipTorque = -(sin(RightHipRAD) * (LF / 2.0) * (MF * G)) * 8;
            }
            else
                RightHipTorque = 0;
        }

        // Left Hip Torque Equation
        if (!dataCore.getLeftProxi())
            LeftHipTorque = (MF * G) * sin(LeftHipRAD) * LF / 2 + (MT * G) * (sin(LeftHipRAD) * LF + sin(LeftKneeRAD - LeftHipRAD) * LT / 2); // TODO : TEST THIS NEW EQUATION
        else
        {
            if (LeftHipAngle < 110)
                LeftHipTorque = -(sin(LeftHipRAD) * (LF / 2.0) * (MF * G)) * 8;
            else
                LeftHipTorque = 0;
        }

        // Right Knee Torque Equation
        if (!dataCore.getRightProxi())
        {
            if (RightKneeAngle > 0)
                RightKneeTorque = ((sin(RightHipRAD) * (LF / 2) * (MF * G)) + ((sin(RightHipRAD) * LF)) * (G * MH)) * 0.5;
            else
                RightKneeAngle = 0;
        }
        else
        {
            if (RightKneeAngle < 110)
                RightKneeTorque = -(sin(RightKneeRAD - RightHipRAD) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
            else
                RightKneeTorque = 0;
        }

        // Left Knee Torque Equation
        if (!dataCore.getLeftProxi())
        {
            if (LeftKneeAngle > 0)
                LeftKneeTorque = ((sin(LeftHipRAD) * (LF / 2) * (MF * G)) + ((sin(LeftHipRAD) * LF)) * (G * MH)) * 0.5;
            else
                LeftKneeTorque = 0;
        }
        else
        {
            if (LeftKneeAngle < 110)
                LeftKneeTorque = -(sin(LeftKneeRAD - LeftHipRAD) * (LT / 2.0) * (MT * G)) * 8; //*4 ajoute pour augmenter force necessaire
            else
                LeftKneeTorque = 0;
        }
    }
    else
    {
        LeftHipTorque = 0;
        RightHipTorque = 0;
        LeftKneeTorque = 0;
        RightKneeTorque = 0;
    }
}

void Logic::printTorque()
{
    Serial.print("  NeededTorqueHL: \t");
    Serial.print(LeftHipTorque);
    Serial.print("  NeededTorqueRH: \t");
    Serial.print(RightHipTorque);
    Serial.print("  NeededTorqueLK: \t");
    Serial.print(LeftKneeTorque);
    Serial.print("  NeededTorqueRK: \t");
    Serial.println(RightKneeTorque);
}

void Logic::printPMW()
{
    Serial.print("\t Motor Power: ");
    Serial.print(dataCore.getMotorPower());
    Serial.print("\t  PWMRightKnee: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::KNEE_R));
    Serial.print("\t  PWMLeftKnee: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::KNEE_L));
    Serial.print("\t  PWMRightHip: ");
    Serial.print(dataCore.getPWM(EnumMotorPosition::HIP_R));
    Serial.print("\t  PWMLeftHip: ");
    Serial.println(dataCore.getPWM(EnumMotorPosition::HIP_L));
}

void Logic::PIDCurrent()
{
    // TODO : In the future, use PID control instead of direct PWM control using map in PIDCurrentPrealable
    //        For now, the PID control is not working properly, so we use the map function to set the PWM values

    /*
      // Set error values on measured current compared to needed current determined by torque
      ErrorCurrentRightKnee = RightKneeNeededCurrent - RightKneeMeasuredCurrent;
      ErrorCurrentLeftKnee = LeftKneeNeededCurrent - LeftKneeMeasuredCurrent;

      // Update integral values depending on calculated errors
      IntegralRightKnee += ErrorCurrentRightKnee;
      IntegralLeftKnee += ErrorCurrentLeftKnee;

      // Update derivative values depending on the rate of change of the calculated errors
      DerivativeRightKnee = ErrorCurrentRightKnee - PreviousErrorRightKnee;
      DerivativeLeftKnee = ErrorCurrentLeftKnee - PreviousErrorLeftKnee;

      // Capping the error values
      LimitMinMax(ErrorCurrentRightKnee, 5);
      LimitMinMax(ErrorCurrentLeftKnee, 5);

      // LimitMinMax(derivative, 50);
      // Setting both PWM values
      PWMRightKnee += KP * ErrorCurrentRightKnee + KI * IntegralRightKnee + KD * DerivativeRightKnee;
      PWMLeftKnee += KP * ErrorCurrentLeftKnee + KI * IntegralLeftKnee + KD * DerivativeLeftKnee;

      // Setting the previous errors for both motors
      PreviousErrorRightKnee = ErrorCurrentRightKnee;
      PreviousErrorLeftKnee = ErrorCurrentLeftKnee;

      // Capping the PWM values for both motors
      LimitMinMax(PWMRightKnee, 255);
      LimitMinMax(PWMLeftKnee, 255);

      //if (angle < 0.2 || angle > (2 * PI - 0.2))
        //PWM = 0;

      */
}

void Logic::neededCurrent()
{
    RightKneeNeededCurrent = (RightKneeTorque / TORQUE2CURRENT) * 1000;
    LeftKneeNeededCurrent = (LeftKneeTorque / TORQUE2CURRENT) * 1000;
    RightHipNeededCurrent = (RightHipTorque / TORQUE2CURRENT) * 1000;
    LeftHipNeededCurrent = (LeftHipTorque / TORQUE2CURRENT) * 1000;
}

void Logic::PIDCurrentPrealable()
{
    int power = dataCore.getMotorPower();

    // Setting  PWM values in dataCore
    dataCore.setPWM(EnumMotorPosition::KNEE_R, -float(map(RightKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power)));
    dataCore.setPWM(EnumMotorPosition::KNEE_L, float(map(LeftKneeTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power)));
    dataCore.setPWM(EnumMotorPosition::HIP_R, float(map(RightHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power)));
    dataCore.setPWM(EnumMotorPosition::HIP_L, -float(map(LeftHipTorque, -HIGH_TORQUE, HIGH_TORQUE, -power, power)));
}

float Logic::toDegrees(float radians)
{
    return radians * 180 / PI;
}

float Logic::toRadian(float degree)
{
    return degree * PI / 180;
}