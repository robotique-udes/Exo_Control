#include <Arduino.h>
#include <Servo.h>
#include <MegunoLink.h>
#include "motorControl.h"
#include "test.h"
#include "touchScreen.h"

SoftwareSerial nextion(2, 3);


#define neutral 0
#define auto 1
#define manuel 2
#define K_RESSORT 0.25
#define DIST_CM 0.245
#define MASSE 0.066
#define PULSE_PAR_TOUR 300
#define TORQUE2CURRENT 6.470
#define KP 0.1
#define KI 0
#define KD 0

Screen ecran;
Motor motor;

long Count_pulses = 0;
float current=0.0;
float I_ressort = 0.0;
float I_gravite = 0.0;
float T_ressort = 0.0;
float T_gravite = 0.0;
float CourantSouhaite = 0.0;
float e = 0.0;
float angle = 0.0;
float integral = 0.0;
float derivative = 0.0;
float previous_error = 0.0;
float PWM = 0.0;


void ReadEncoder();
void CapperFloat(float &val, float cap);

void setup() {
  
  Serial.begin(9600);
  nexInit();  
  
  pinMode(31 , INPUT_PULLUP);
  pinMode(34 , INPUT_PULLUP);
  pinMode(EN , OUTPUT);
  pinMode(IN1 , OUTPUT);
  pinMode(IN2 , OUTPUT);
  pinMode(CT, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(ENCA, INPUT);

Serial.println("Ini motor exo----------");

  attachInterrupt(digitalPinToInterrupt(ENCA), ReadEncoder, RISING);
}

void loop() 
{
    //ecran.nextLoop();
    //Get angle
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



    //Trouver valeur du courant pour gravité
    T_gravite = DIST_CM * MASSE * 9.81 * sin(angle);
    CourantSouhaite = T_gravite / TORQUE2CURRENT * 1000;


    //PID for current 
    e = CourantSouhaite - motor.ReadCurrent();
   
    integral = integral + e;
    derivative = e - previous_error;
    CapperFloat(e, 5);
    //CapperFloat(derivative, 50);
    PWM += KP*e + KI*integral + KD*derivative;
    previous_error = e;

    CapperFloat(PWM, 255);
    if(angle<0.2 || angle>(2*PI-0.2))
      PWM = 0;


    //Set PWM
    motor.motorSetSpeed(PWM);


    Serial.print("Courant RESSORT: ");
    Serial.print(I_ressort);
    Serial.print(" Courant GRAVITE: ");
    Serial.print(I_gravite);
    Serial.print(" derivate: ");
    Serial.print(derivative);
    Serial.print(" Courant actuel: ");
    Serial.print(motor.ReadCurrent());
    Serial.print(" Angle: ");
    Serial.print(angle);
    Serial.print(" Count_pulses: ");
    Serial.print(Count_pulses);
    Serial.print(" PWM: ");
    Serial.println(PWM);
    delay(10);

}

void ReadEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    Count_pulses++;
  } else {
    Count_pulses--;
  }
}

void CapperFloat(float &val, float max)
{
  if(val > max)
    val = max;
  else if(val < -max)
    val = -max;
}
