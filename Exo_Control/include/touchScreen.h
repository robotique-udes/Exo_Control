
#include <Arduino.h>
#include <Nextion.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <string.h>
using namespace std;
#define neutral 0;
#define auto 1;
#define manuel 2;
SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

class Screen
{
    private:
        int mode;

        //---------------------------------------BOUTONS (page, id, name)--------------------------------------------
        NexButton bON = NexButton(0, 4, "b0"); // STATE ON
        NexButton bOFF = NexButton(0, 5, "b1"); // STATE OFF

        NexButton bLeft_Hip_UP = NexButton(1, 14, "b9"); // Left Hip UP
        NexButton bLeft_Hip_DOWN = NexButton(1, 13, "b8"); // Left Hip Down
        NexButton bRight_Hip_UP = NexButton(1, 16, "b11"); // Right Hip UP
        NexButton bRight_Hip_DOWN = NexButton(1, 15, "b10"); // Right Hip Down

        NexButton bLeft_Knee_RETRACT = NexButton(1, 9, "b0"); // Left Hip UP
        NexButton bLeft_Knee_EXTRACT = NexButton(1, 10, "b1"); // Left Hip Down
        NexButton bRight_Knee_RETRACT = NexButton(1, 12, "b7"); // Right Hip UP
        NexButton bRight_Knee_EXTRACT = NexButton(1, 11, "b6"); // Right Hip Down

        NexButton bMAIN_page = NexButton(0, 6, "b2") = NexButton(1, 2, "b2") = NexButton(2, 2, "b2") = NexButton(3, 2, "b2"); //Page 0
        NexButton bROBOT_page = NexButton(0, 7, "b3") = NexButton(1, 3, "b3") = NexButton(2, 3, "b3") = NexButton(3, 3, "b3"); //Page 1
        NexButton bSETTINGS_page = NexButton(0, 8, "b4") = NexButton(1, 4, "b4") = NexButton(2, 4, "b4") = NexButton(3, 4, "b4"); //Page 2
        NexButton bGOOSE_page = NexButton(0, 9, "b5") = NexButton(1, 5, "b5") = NexButton(2, 5, "b5") = NexButton(3, 5, "b5"); //Page 3


        //---------------------------------------TEXTES--------------------------------------------
        NexText t0 = NexText(0, 3, "t0"); //TEXTE STATE

        //---------------------------------------NOTIFS--------------------------------------------

        NexTouch *nex_listen_list[15] = { //notif quand appele

            &bMAIN_page, //page 0
            &bROBOT_page, //page 1
            &bSETTINGS_page, //page 2
            &bGOOSE_page, //page 3

            &bON, //stateON (Auto)
            &bOFF, //stateOFF (Neutra,)

            &bLeft_Hip_UP, //HIPS
            &bLeft_Hip_DOWN,
            &bRight_Hip_UP,
            &bRight_Hip_DOWN,

            &bLeft_Knee_RETRACT, //KNESS
            &bLeft_Knee_EXTRACT,
            &bRight_Knee_RETRACT,
            &bRight_Knee_EXTRACT,
            NULL
        };

    public:
        Screen();
        ~Screen();

        void StateON(void *ptr);

//STATE NEUTRAL (0)
void StateOFF(void *ptr);

//---------------------------------------HIPS--------------------------------------------
//Left
void LeftHip_UP(void *ptr);

void LeftHip_DOWN(void *ptr);

void LeftHip_NEUTRAL(void *ptr);

//Right
void RightHip_UP(void *ptr);

void RightHip_DOWN(void *ptr);

void RightHip_NEUTRAL(void *ptr);

//---------------------------------------KNEES--------------------------------------------
//Left
void LeftKnee_RETRACT(void *ptr);

void LeftKnee_EXTRACT(void *ptr);

void LeftKnee_NEUTRAL(void *ptr);

//Right
void RightKnee_RETRACT(void *ptr);

void RightKnee_EXTRACT(void *ptr);

void RightKnee_NEURAL(void *ptr);

//---------------------------------------PAGES--------------------------------------------
void MainPage(void *ptr);
void RobotPage(void *ptr);

void SettingsPage(void *ptr);

void GoosePage(void *ptr);

void nextLoop();

};