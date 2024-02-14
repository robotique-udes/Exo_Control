#include "relay.h"


void Relay::setPins()
{
  pinMode(RELAIS_GENOU_GAUCHE, OUTPUT);
  pinMode(RELAIS_GENOU_DROIT, OUTPUT);
  pinMode(RELAIS_HANCHE_GAUCHE, OUTPUT);
  pinMode(RELAIS_HANCHE_DROITE, OUTPUT);
}

void Relay::setRelais(int ID_Relais, bool state)
{
  Serial.println(ID_Relais);
  if (state != ON)
    digitalWrite(ID_Relais, HIGH);
  else
    digitalWrite(ID_Relais, LOW); 
}
void Relay::setAllRelais(bool state)
{
  Serial.print("switching all relay to: ");
  Serial.println(state);
   setRelais(RELAIS_GENOU_GAUCHE, state);
   setRelais(RELAIS_GENOU_DROIT,state);
   setRelais(RELAIS_HANCHE_DROITE,state);
   setRelais(RELAIS_HANCHE_GAUCHE,state);
   
}