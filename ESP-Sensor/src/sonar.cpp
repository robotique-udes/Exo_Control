#include "sonar.h"

double height = 40;
bool RightSonarState = true;
bool LeftSonarState = true;


Sonar::Sonar()
{
}

Sonar::~Sonar()
{
}

void Sonar::setHeight(double h)
{
  height = h;
  Serial.print("Height set to: ");
  Serial.println(h);
}

double Sonar::getHeight()
{
  return height;
}

//RETURN DISTANCE READING FROM RIGHT SONAR
double Sonar::sonarScanR()
{
  //Signal acquisition from right sonar
  digitalWrite(SDA_PIN_DROIT, LOW);
  delayMicroseconds(5);
  digitalWrite(SDA_PIN_DROIT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SDA_PIN_DROIT, LOW);
  double RightDuration = pulseIn(SCL_PIN_DROIT, HIGH);

  return (RightDuration / 2) / 29.1;
}

//RETURN DISTANCE READING FROM LEFT SONAR
double Sonar::sonarScanL()
{
  //Signal acquisition from left sonar
  digitalWrite(SDA_PIN_GAUCHE, LOW);
  delayMicroseconds(5);
  digitalWrite(SDA_PIN_GAUCHE, HIGH);
  delayMicroseconds(10);
  digitalWrite(SDA_PIN_GAUCHE, LOW);
  double LeftDuration = pulseIn(SCL_PIN_GAUCHE, HIGH);
  return (LeftDuration / 2) / 29.1;

}
void Sonar::setSonarState(bool state)
{
  RightSonarState = state;
  LeftSonarState = state;
}


void Sonar::sonarRead()
{

  float errorRight = 0;
  float errorLeft = 0;


//Determining the current SonarState for each sensor
Serial.print("Sonar height R: ");
Serial.print(sonarScanR());
Serial.print(" Sonar height L: ");
Serial.print(sonarScanL());
//Here, right sensor is examined 
  if (!RightSonarState)
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanR() < height)
        errorRight += 1;
    errorRight /= iteration;
    if (errorRight <= UNCERTAINTY)
      RightSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanR() > height)
        errorRight += 1;
    errorRight /= iteration;
    if (errorRight <= UNCERTAINTY)
      RightSonarState = false;
  }

//Here, left sensor is examined
   if (!LeftSonarState)
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanL() < height)
        errorLeft += 1;
    errorLeft /= iteration;
    if (errorLeft <= UNCERTAINTY)
      LeftSonarState = true;
  }
  else
  {
    for (int i = 0; i < iteration; i++)
      if (sonarScanL() > height)
        errorLeft += 1;
    errorLeft /= iteration;
    if (errorLeft <= UNCERTAINTY)
      LeftSonarState = false;
  }
  
  LeftSonarState = !LeftSonarState;
  RightSonarState = !RightSonarState;


}

    bool Sonar::getSonarStateL()
    {
        return LeftSonarState;
    }
    bool Sonar::getSonarStateR()
    {
        return RightSonarState;
    }
