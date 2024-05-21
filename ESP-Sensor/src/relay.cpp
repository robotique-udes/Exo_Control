#include "relay.h"


void Relay::setPins()
{
  pinExtender.pinMode(RELAIS_GENOU_GAUCHE, OUTPUT);
  pinExtender.pinMode(RELAIS_GENOU_DROIT, OUTPUT);
  pinExtender.pinMode(RELAIS_HANCHE_GAUCHE, OUTPUT);
  pinExtender.pinMode(RELAIS_HANCHE_DROITE, OUTPUT);
}

void Relay::setRelais(int ID_Relais, bool state)
{
  if (state != ON)
    pinExtender.digitalWrite(ID_Relais, HIGH);
  else
    pinExtender.digitalWrite(ID_Relais, LOW); 
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