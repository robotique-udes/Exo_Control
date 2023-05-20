#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>
#include <SPI.h>
//#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <string.h>
#include "motorControl.h"
using namespace std;

#define RX_HMI 16
#define TX_HMI 17



class Screen
{
    private:
        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        static NexButton bDISARMED; // Disarmed
        static NexButton bARMED;    // Armed
        static NexButton bMOTORISED;// Motorised

        static NexButton bWalk; // Standing Button
        static NexButton bSquat; // Squatting Button

        static NexButton bAutoCalib; // Autocalibrating Button
        static NexButton b4f ; // GOOSE
        static NexButton b5f8;
        static NexButton b5f10;
        static NexButton b6f ;  

        static NexButton bPID0;
        static NexButton bPID1;
        static NexButton bPID2;
        static NexButton bSavePID;

        static NexSlider Pslider;
        static NexNumber Pindicator;


        //---------------------------------------FONCTIONS-----------------------------------------
        static void Disarmed(void *ptr);

        static void Armed(void *ptr);

        static void Motorised(void *ptr);

        //---------------------------------------TESTS--------------------------------------------
        static void TestWalk(void *ptr);

        static void TestSquat(void *ptr);

        static void TestNeutral(void *ptr);

        //---------------------------------------CALIBRATION--------------------------------------------
        static void AutoCalibration(void *ptr);

        static void Quack(void *ptr);

        static void Calib_5f8(void *ptr);

        static void Calib_5f10(void *ptr);

        static void Calib_6f(void *ptr);

        //---------------------------------------PID--------------------------------------------
        static void PID(void *ptr);

        static void SavePID(void *ptr);

        static void Pvalue(void *ptr);

        static NexTouch *nex_listen_list[];

    public:
        static Motor *motor;
        Screen();
        ~Screen();
        static void nextLoop();


};



#endif 