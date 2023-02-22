
#include <Arduino.h>
#include <Nextion.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <string.h>
#include "touchScreen.cpp"
using namespace std;
int mode;
#define neutral 0;
#define auto 1;
#define manuel 2;