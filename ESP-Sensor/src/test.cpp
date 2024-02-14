#include "test.h"

void Test::testSonar()
{
    float erreur = 0;
    duration = pulseIn(echoPin, HIGH);

    cm = (duration/2) / 29.1;

    inches = (duration/2) / 74;
    if (state==false)
    {
        for (int i =0; i<10;i++) if (cm<height) erreur+=1;
        erreur=erreur/10;
        if(erreur<=0.2) state=true;
    }
    else
    {
        for (int i=0; i<10;i++) if (cm>height) erreur+=1;
        erreur=erreur/10;
        if(erreur<=0.2) state=false;
    }
    if(state==false)
    {
        Serial.println("Sol");
    }
    else
    {
        Serial.println("      Air");
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

void Test::testRelay()
{
    relais.setAllRelais(ON);
    delay(500);
    relais.setAllRelais(OFF);
    delay(500);
    Serial.println("Relais 1 ");
    relais.setRelais(RELAIS_HANCHE_GAUCHE, ON);
    delay(500);
    relais.setRelais(RELAIS_HANCHE_GAUCHE, OFF);
    Serial.println("Relais 2 ");
    relais.setRelais(RELAIS_HANCHE_DROITE, ON);
    delay(500);
    relais.setRelais(RELAIS_HANCHE_DROITE, OFF);
    Serial.println("Relais 3 ");
    relais.setRelais(RELAIS_GENOU_GAUCHE, ON);
    delay(500);
    relais.setRelais(RELAIS_GENOU_GAUCHE, OFF);
    Serial.println("Relais 4 ");
    relais.setRelais(RELAIS_GENOU_DROIT, ON);
    delay(500);
    relais.setRelais(RELAIS_GENOU_DROIT, OFF);

}