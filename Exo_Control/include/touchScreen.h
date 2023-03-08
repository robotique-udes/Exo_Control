#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <Nextion.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <string.h>
#include "motorControl.h"
using namespace std;
#define neutral 0;
#define auto 1;
#define manuel 2;
// Nextion TX to pin 2 and RX to pin 3 of Arduino

class Screen
{
    private:

        static Motor moteur01;
        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        static NexButton bON; // STATE ON
        static NexButton bOFF; // STATE OFF

        static NexButton bLeft_Hip_UP; // Left Hip UP
        static NexButton bLeft_Hip_DOWN; // Left Hip Down
        static NexButton bRight_Hip_UP; // Right Hip UP
        static NexButton bRight_Hip_DOWN; // Right Hip Down

        static NexButton bLeft_Knee_RETRACT; // Left Hip UP
        static NexButton bLeft_Knee_EXTRACT ; // Left Hip Down 
        static NexButton bRight_Knee_RETRACT; // Right Hip UP
        static NexButton bRight_Knee_EXTRACT; // Right Hip Down

        static NexButton bMAIN_page; //Page 0
        static NexButton bROBOT_page; //Page 1
        static NexButton bSETTINGS_page; //Page 2
        static NexButton bGOOSE_page; //Page 3


        //---------------------------------------TEXTES--------------------------------------------
        static NexText t0; //TEXTE STATE
        //---------------------------------------FONCTIONS-----------------------------------------
        static void StateON(void *ptr);

//STATE NEUTRAL (0)
        static void StateOFF(void *ptr);

        //---------------------------------------HIPS--------------------------------------------
        //Left
        static void LeftHip_UP(void *ptr);

        static void LeftHip_DOWN(void *ptr);

        static void LeftHip_NEUTRAL(void *ptr);

        //Right
        static void RightHip_UP(void *ptr);

        static void RightHip_DOWN(void *ptr);

        static void RightHip_NEUTRAL(void *ptr);

        //---------------------------------------KNEES--------------------------------------------
        //Left
        static void LeftKnee_RETRACT(void *ptr);

        static void LeftKnee_EXTRACT(void *ptr);

        static void LeftKnee_NEUTRAL(void *ptr);

        //Right
        static void RightKnee_RETRACT(void *ptr);

        static void RightKnee_EXTRACT(void *ptr);

        static void RightKnee_NEURAL(void *ptr);

        //---------------------------------------PAGES--------------------------------------------
        static void MainPage(void *ptr);
        static void RobotPage(void *ptr);

        static void SettingsPage(void *ptr);

        static void GoosePage(void *ptr);



        static NexTouch *nex_listen_list[];

    public:
    
        Screen();
        ~Screen();

        static void nextLoop();
        static int mode;

};

#endif 