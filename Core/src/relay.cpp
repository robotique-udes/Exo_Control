#include "relay.h"


void Relay::setPins()
{
  pinExtender.pinMode(RELAIS_GENOU_GAUCHE, OUTPUT);
  pinExtender.pinMode(RELAIS_GENOU_DROIT, OUTPUT);
  pinExtender.pinMode(RELAIS_HANCHE_GAUCHE, OUTPUT);
  pinExtender.pinMode(RELAIS_HANCHE_DROITE, OUTPUT);
}

void Relay::setRelay(int ID_Relais, bool state)
{
  if (state != ON)
    pinExtender.digitalWrite(ID_Relais, HIGH);
  else
    pinExtender.digitalWrite(ID_Relais, LOW); 
}

void Relay::setAllRelay(bool state)
{
  Serial.print("switching all relay to: ");
  Serial.println(state);
  setRelay(RELAIS_GENOU_GAUCHE, state);
  setRelay(RELAIS_GENOU_DROIT,state);
  setRelay(RELAIS_HANCHE_DROITE,state);
  setRelay(RELAIS_HANCHE_GAUCHE,state);
   
}