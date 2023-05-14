#include "motorControl.h"

//ExponentialFilter<float> FilteredCurrent(7, 0);

Motor::Motor()
{}

Motor::~Motor()
{}

//Servo MonServo;

/*Fonctions de torque a integrer

TgSol = (((cos(angleGenoux)*LF) - 0.5*LH*cos(angleHanche))*Fgh) + (0.5*LF*cos(angleGenoux)*Fgf);
TgVide = (cos(angleGenoux)*Fgt*LG)/2;
ThSol = (0.5*LH*cos(angleHanche)*Fgh);
ThVide = (cos(angleHanche)*LH*Fgf)/2 + (cos(angleHanche)*LF + (cos(angleGenoux)*LG)/2)*Fgt;
*/


void Motor::motorSetSpeed(int val)
{
  if(val >= 0)
  {
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    Rotation = ClockWise;
  }
  else if(val < 0)
  {
    digitalWrite(IN2 , LOW);
    digitalWrite(IN1 , HIGH);
    Rotation = CounterClockWise;
    val = -val;
  }

    analogWrite(EN , val);
}

void Motor::motorOn()
{
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    analogWrite(EN , 150);
}
void Motor::motorOff()
{
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN1 , LOW);
    analogWrite(EN , 0);
}
float Motor::ReadCurrent()
{
    CTval = analogRead(CT);
    current = ((CTval * 27.0) / 1023.0) * 1000.0;
    //FilteredCurrent.Filter(current);
    //return FilteredCurrent.Current()*Rotation;
    return current;
}

void Motor::setAngle( long Count_pulses)
{
      angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
    if(angle > 2*PI)
    {
      Count_pulses -= PULSE_PAR_TOUR;
      angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
      }
    else if (angle < 0)
    {
      Count_pulses +=  PULSE_PAR_TOUR;
      angle = Count_pulses * 2*PI /PULSE_PAR_TOUR;
    }
}

void Motor::CapperFloat(float &val, float max)
{
  if(val > max)
    val = max;
  else if(val < -max)
    val = -max;
}

void Motor::neededTorque()
{
    //Trouver valeur du courant pour gravité
    T_gravite = DIST_CM * MASSE * 9.81 * sin(angle);
    CourantSouhaite = T_gravite / TORQUE2CURRENT * 1000;
}

float Motor::neededCurrent()
{
      //PID for current 
    e = CourantSouhaite - ReadCurrent();
   
    integral = integral + e;
    derivative = e - previous_error;
    CapperFloat(e, 5);
    //CapperFloat(derivative, 50);
    PWM += KP*e + KI*integral + KD*derivative;
    previous_error = e;

    CapperFloat(PWM, 255);
    if(angle<0.2 || angle>(2*PI-0.2))
      PWM = 0;
    
    return PWM;

}

void Motor::printData(long Count_pulses)
{
    // Serial.print(" Angle: ");
    // Serial.print(angle);
    Serial.print(" Courant GRAVITE: ");
    Serial.print(CourantSouhaite);
    // Serial.print(" derivate: ");
    // Serial.print(derivative);
    Serial.print(" Courant actuel: ");
    Serial.print(ReadCurrent());
    // Serial.print(" Count_pulses: ");
    // Serial.print(Count_pulses);
    Serial.print(" PWM: ");
    Serial.println(PWM);
}

bool Motor::sonarRead()
{
    float erreur = 0;
    duration = pulseIn(echoPin, HIGH);

    cm = (duration/2) / 29.1;

    inches = (duration/2) / 74;
    if (state==false)
    {
        for (int i =0; i<iteration;i++) if (cm<height) erreur+=1;
        erreur=erreur/iteration;
        if(erreur<=0.2) state=true;
    }
    else
    {
        for (int i=0; i<iteration;i++) if (cm>height) erreur+=1;
        erreur=erreur/iteration;
        if(erreur<=0.2) state=false;
    }
    if(state==false)
    {
        //Serial.println("Sol");
        return true;
    }
    else
    {
        //Serial.println("      Air");
        return false;
    }

    //Serial.print(inches);

    //Serial.print("in, ");
    //Serial.print("State: ");
    //Serial.println(state);
    //Serial.print(cm);

    //Serial.print("cm");
    //Serial.println();
    //delay(200);

}