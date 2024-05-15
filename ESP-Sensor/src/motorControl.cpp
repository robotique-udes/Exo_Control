#include "motorControl.h"
bool motorMode = OFF; //mutliplie le torque demandé au moteur, lorsque a 0, les moteur sont effectivement à Off, contrôlé avec le HMI ou hardcode pour test
double power=175;

Motor::Motor()
{
  
}

Motor::~Motor()
{
}

void Motor::setPins()
{
  // PINS MOTEURS
  pinMode(D1_CT_A, INPUT);
  pinMode(D2_CT_A, INPUT);
}
void Motor::readCurrent()
{
  int CTLeftKnee = D1_CT_A;;
  int CTRightKnee = D2_CT_A;
  float CTRightKneeValue = 0.0;
  float CTLeftKneeValue = 0.0;

  CTRightKneeValue = analogRead(CTRightKnee);
  CTLeftKneeValue = analogRead(CTLeftKnee);

  RightKneeMeasuredCurrent = ((CTRightKneeValue * 27.0) / 1023.0) * 1000.0;
  LeftKneeMeasuredCurrent = ((CTLeftKneeValue * 27.0) / 1023.0) * 1000.0; 
}


void Motor::CapperFloat(float &val, float max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::CapperInt(int &val, int max)
{
  if (val > max)
    val = max;
  else if (val < -max)
    val = -max;
}

void Motor::neededTorque()
{
  //If clutch are on automatic, calculate torque needed
  if(motorMode){
    // Right Hip Torque Equation
    if (!RightProxim.IsOnTheGround())
      RightHipTorque =0;
    else
    {
      if(toDegrees(RightHipAngle)<110)
      {
        RightHipTorque = -(sin(RightHipAngle)*(LF/2.0)*(MF*G))*8;
      }
      else
        RightHipTorque = 0;
    }

    // Left Hip Torque Equation
    if (!LeftProxim.IsOnTheGround())
        LeftHipTorque = 0;
    else
    {
      if(toDegrees(LeftHipAngle)<110)
        LeftHipTorque = -(sin(LeftHipAngle)*(LF/2.0)*(MF*G))*8;  
      else
        LeftHipTorque = 0;
    }  

    // Right Knee Torque Equation
    if (!RightProxim.IsOnTheGround())
    {
      if(toDegrees(RightKneeAngle)>0)
        RightKneeTorque = ((sin(RightHipAngle)*(LF/2)*(MF*G)) + ((sin(RightHipAngle)*LF))*(G*MH))*0.5;
      else
        RightKneeAngle = 0;
    }
    else
    {
      if(toDegrees(RightKneeAngle)<110)
        RightKneeTorque = -(sin(RightKneeAngle - RightHipAngle)*(LT/2.0)*(MT*G)) *8 ; //*4 ajoute pour augmenter force necessaire
      else
        RightKneeTorque = 0;
    } 
      
    // Left Knee Torque Equation
    if (!LeftProxim.IsOnTheGround())
    {
      if(toDegrees(LeftKneeAngle)>0)
        LeftKneeTorque = ((sin(LeftHipAngle)*(LF/2)*(MF*G)) + ((sin(LeftHipAngle)*LF))*(G*MH))*0.5;
      else
        LeftKneeTorque = 0;
    }
    else
    {
      if(toDegrees(LeftKneeAngle)<110)
        LeftKneeTorque = -(sin(LeftKneeAngle - LeftHipAngle)*(LT/2.0)*(MT*G))*8 ; //*4 ajoute pour augmenter force necessaire
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

void Motor::printTorque()
{
  Serial.print("  NeededTorqueHL: ");
  Serial.print(LeftHipTorque);
  Serial.print("  NeededTorqueRH: ");
  Serial.print(RightHipTorque);
  Serial.print("  NeededTorqueLK: ");
  Serial.print(LeftKneeTorque);
  Serial.print("  NeededTorqueRK: ");
  Serial.print(RightKneeTorque);
}

void Motor::neededCurrent()
{
  //neededCurrent = SideKneeTorque / TORQUE2CURRENT * 1000;

RightKneeNeededCurrent = (RightKneeTorque/TORQUE2CURRENT)*1000;
LeftKneeNeededCurrent = (LeftKneeTorque/TORQUE2CURRENT)*1000;

}

void Motor::PIDCurrent()
{
//Set error values on measured current compared to needed current determined by torque
  ErrorCurrentRightKnee = RightKneeNeededCurrent - RightKneeMeasuredCurrent;
  ErrorCurrentLeftKnee = LeftKneeNeededCurrent - LeftKneeMeasuredCurrent;

//Update integral values depending on calculated errors
  IntegralRightKnee += ErrorCurrentRightKnee;
  IntegralLeftKnee += ErrorCurrentLeftKnee;

//Update derivative values depending on the rate of change of the calculated errors 
  DerivativeRightKnee = ErrorCurrentRightKnee - PreviousErrorRightKnee;
  DerivativeLeftKnee = ErrorCurrentLeftKnee - PreviousErrorLeftKnee;

//Capping the error values
  CapperFloat(ErrorCurrentRightKnee, 5);
  CapperFloat(ErrorCurrentLeftKnee, 5);

  // CapperFloat(derivative, 50);
//Setting both PWM values 
  PWMRightKnee += KP * ErrorCurrentRightKnee + KI * IntegralRightKnee + KD * DerivativeRightKnee;
  PWMLeftKnee += KP * ErrorCurrentLeftKnee + KI * IntegralLeftKnee + KD * DerivativeLeftKnee;

//Setting the previous errors for both motors
  PreviousErrorRightKnee = ErrorCurrentRightKnee;
  PreviousErrorLeftKnee = ErrorCurrentLeftKnee;

//Capping the PWM values for both motors
  CapperInt(PWMRightKnee, 255);
  CapperInt(PWMLeftKnee, 255);

  /*if (angle < 0.2 || angle > (2 * PI - 0.2))
    PWM = 0;*/

}

void Motor::PIDCurrentPrealable()
{

//Setting  PWM values 
  PWMRightKnee = map(RightKneeTorque, -100, 100, -power, power);
  PWMLeftKnee = map(LeftKneeTorque, -100, 100, -power, power);
  PWMRightHip = map(RightHipTorque, -100, 100, -power, power);
  PWMLeftHip = map(LeftHipTorque, -100, 100, -power, power);

}

void Motor::printProxim()
{
  
  Serial.print(" SL: ");
  Serial.print(LeftProxim.IsOnTheGround());
  Serial.print(" SR: ");
  Serial.print(RightProxim.IsOnTheGround());
  Serial.print(" MM: ");
  Serial.print(motorMode);
}

void Motor::setMotorMode(bool state)
{
   motorMode = state;
}

void Motor::setPower(double p)
{power = p;}
double Motor::getPower()
{return power;}


void Motor::setAngle(enumIMU imuType, float val)
{
  switch (imuType)
  {
    case enumIMU::HipR:
        RightHipAngle = val;
        break;
    case enumIMU::KneeR:
        RightKneeAngle = val;
        break;
    case enumIMU::HipL:
        LeftHipAngle = val;
        break;
    case enumIMU::KneeL:
        LeftKneeAngle = val;
        break;
    default:
        break;
  }
}

float Motor::toDegrees(float radians)
{
    return radians * 180 / PI;
}

//A renommer correctement
void Motor::setSonarState(bool state){ settings.setState(state); }
void Motor::setHeight(double h){ settings.setHeight(h); }
double Motor::sonarScanR(){ return RightProxim.GetMinDistance(); }
double Motor::sonarScanL(){ return LeftProxim.GetMinDistance(); }
//Fpnction probablement obscelete, a revisiter
void Motor::sonarRead(){ /*sonar.sonarRead();*/}



