#include "motorControl.h"



Motor::Motor()
{}

Motor::~Motor()
{}

void Motor::motorSetSpeed(int ID, int in1, int in2, int val)
{
  int IN1;
  int IN2;
  int EN;

  if (ID == MOTEUR_GENOU_GAUCHE)
  {
    IN1 = D1_IN1_A;
    IN2 = D1_IN2_A;
    EN = D1_EN_A;
  }
  else if (ID == MOTEUR_HANCHE_GAUCHE)
  {
    IN1 = D2_IN1_A;
    IN2 = D2_IN2_A;
    EN = D2_EN_A;
  }
  else if (ID == MOTEUR_GENOU_DROIT )
  {
    IN1 = D1_IN1_B;
    IN2 = D1_IN2_B;
    EN = D1_EN_B;
  }
  else if (ID == MOTEUR_HANCHE_DROITE)
  {
    IN1 = D2_IN1_B;
    IN2 = D2_IN2_B;
    EN = D2_EN_B;
  }
  else
  {
    Serial.println("ERROR : ID MOTEUR INCORRECT");
  }

  digitalWrite(IN1 , in1);
  digitalWrite(IN2 , in2);
  analogWrite(EN, val);


}