#include "test.h"


void Test::setMotor(Motor *newMotor)
{
    motor = newMotor;
}

void Test::setEncoder(QuadratureEncoder *newEncoder)
{
    encoder = newEncoder;
}

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

void Test::keyboardCommand()
{
  if (Serial.available() > 0)
  {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // Check the value of the incoming byte and print the corresponding message
    if (incomingByte == 'q')
    {
      Serial.println("MOTEUR_HANCHE_DROITE");
      motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 2000);
    }
    else if (incomingByte == 'a')
    {
      Serial.println("MOTEUR_HANCHE_DROITE");
      motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, -2000);
    }
    else if (incomingByte == 'w')
    {
      Serial.println("MOTEUR_GENOU_DROIT");
      motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 2000);
    }
    else if (incomingByte == 's')
    {
      Serial.println("MOTEUR_GENOU_DROIT");
      motor->motorSetSpeed(MOTEUR_GENOU_DROIT, -2000);
    }
    else if (incomingByte == 'e')
    {
      Serial.println("MOTEUR_HANCHE_GAUCHE");
      motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 2000);
    }
    else if (incomingByte == 'd')
    {
      Serial.println("MOTEUR_HANCHE_GAUCHE");
      motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, -2000);
    }
    else if (incomingByte == 'r')
    {
      Serial.println("MOTEUR_GENOU_GAUCHE");
      motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 2000);
    }
    else if (incomingByte == 'f')
    {
      Serial.println("MOTEUR_GENOU_GAUCHE");
      motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, -2000);
    }
    else if (incomingByte == ' ')
    {
      Serial.println("stop");
      motor->motorSetSpeed(MOTEUR_HANCHE_DROITE, 0);
      motor->motorSetSpeed(MOTEUR_GENOU_DROIT, 0);
      motor->motorSetSpeed(MOTEUR_HANCHE_GAUCHE, 0);
      motor->motorSetSpeed(MOTEUR_GENOU_GAUCHE, 0);
    }
    else if (incomingByte == '1')
    {
      Serial.println("Relais 1 ON");
      relais.setRelais(RELAIS_HANCHE_DROITE, ON);
    } 
    else if (incomingByte == '2')
    {
      Serial.println("Relais 2 OFF");
      relais.setRelais(RELAIS_HANCHE_DROITE, OFF);
    }
    else if (incomingByte == '3')
    {
      Serial.println("Relais 2 ON");
      relais.setRelais(RELAIS_GENOU_DROIT, ON);
    }
    else if (incomingByte == '4')
    {
      Serial.println("Relais 2 OFF");
      relais.setRelais(RELAIS_GENOU_DROIT, OFF);
    }
    else if (incomingByte == '5')
    {
      Serial.println("Relais 3 ON");
      relais.setRelais(RELAIS_HANCHE_GAUCHE, ON);
    }
    else if (incomingByte == '6')
    {
      Serial.println("Relais 3 OFF");
      relais.setRelais(RELAIS_HANCHE_GAUCHE, OFF);
    }
    else if (incomingByte == '7')
    {
      Serial.println("Relais 4 ON");
      relais.setRelais(RELAIS_GENOU_GAUCHE, ON);
    }
    else if (incomingByte == '8')
    {
      Serial.println("Relais 4 OFF");
      relais.setRelais(RELAIS_GENOU_GAUCHE, OFF);
    }
    else if (incomingByte == '9')
    {
      Serial.println("Relais ALL ON");
      relais.setAllRelais(ON);
    }
    else if (incomingByte == '0')
    {
      Serial.println("Relais ALL OFF");
      relais.setAllRelais(OFF);
    }
    else if (incomingByte == 'z')
    {
      Serial.println("Reset Encoder Hanche Droite");
      encoder->resetPosition(QuadratureEncoder::HAN_DRO);
    }
    else if (incomingByte == 'x')
    {
      Serial.println("Reset Encoder Hanche Gauche");
      encoder->resetPosition(QuadratureEncoder::HAN_GAU);
    }
    else if (incomingByte == 'c')
    {
      Serial.println("Reset Encoder Genou Droit");
      encoder->resetPosition(QuadratureEncoder::GEN_DRO);
    }
    else if (incomingByte == 'v')
    {
      Serial.println("Reset Encoder Genou Gauche");
      encoder->resetPosition(QuadratureEncoder::GEN_GAU);
    }
    else if (incomingByte == '-')
    {
      Serial.println("Reset Encoder Genou Gauche");
      motor->setSonarTrigger();
    }
    
  }
}

void Test::printEncoder()
{
    Serial.print("Position Hanche Droite: ");
    Serial.println(encoder->getPositionPulses(QuadratureEncoder::HAN_DRO));
    Serial.print("Position Hanche Gauche: ");
    Serial.println(encoder->getPositionPulses(QuadratureEncoder::HAN_GAU));
    Serial.print("Position Genou Droit: ");
    Serial.println(encoder->getPositionPulses(QuadratureEncoder::GEN_DRO));
    Serial.print("Position Genou Gauche: ");
    Serial.println(encoder->getPositionPulses(QuadratureEncoder::GEN_GAU));

    Serial.print("Position Hanche Droite: ");
    Serial.println(encoder->getPositionAngle(QuadratureEncoder::HAN_DRO));
    Serial.print("Position Hanche Gauche: ");
    Serial.println(encoder->getPositionAngle(QuadratureEncoder::HAN_GAU));
    Serial.print("Position Genou Droit: ");
    Serial.println(encoder->getPositionAngle(QuadratureEncoder::GEN_DRO));
    Serial.print("Position Genou Gauche: ");
    Serial.println(encoder->getPositionAngle(QuadratureEncoder::GEN_GAU));
}