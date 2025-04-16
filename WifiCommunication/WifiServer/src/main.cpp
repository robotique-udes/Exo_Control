#include"Arduino.h"
#include"WifiServer.h"
#include "MessageBuilder/MessageBuilder.h"
#include "enums.h"
#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

#define BUTTON_SIZE 35
#define ROW1 75
#define ROW2 150

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>

void TestDeserializeData();
void TestSendingtoExo();

WifiServer* wifiserver;

unsigned long previousMillis = 0; // Stores the last time a message was printed
const unsigned long interval = 1000; // Interval between prints (1 second)

void btn_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        // /*Get the first child of the button which is the label and change its text*/
        // lv_obj_t *label = lv_obj_get_child(btn, 0);
        // lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void makeButtons_Row1()
{
  lv_obj_t *btn = lv_btn_create(lv_scr_act());                /*Add a button the current screen*/
  lv_obj_set_pos(btn, 200, ROW1);                                /*Set its position*/
  lv_obj_set_size(btn, BUTTON_SIZE, BUTTON_SIZE);                              /*Set its size*/
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
  lv_label_set_text(label, "+");     /*Set the labels text*/
  lv_obj_center(label);

  lv_obj_t *btn2 = lv_btn_create(lv_scr_act());                /*Add a button the current screen*/
  lv_obj_set_pos(btn2, 150, ROW1);                                /*Set its position*/
  lv_obj_set_size(btn2, BUTTON_SIZE, BUTTON_SIZE);                              /*Set its size*/
  lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label2 = lv_label_create(btn2); /*Add a label to the button*/
  lv_label_set_text(label2, "-");     /*Set the labels text*/
  lv_obj_center(label2);
}

void makeButtons_Row2()
{
  lv_obj_t *btn = lv_btn_create(lv_scr_act());                /*Add a button the current screen*/
  lv_obj_set_pos(btn, 200, ROW2);                                /*Set its position*/
  lv_obj_set_size(btn, BUTTON_SIZE, BUTTON_SIZE);                              /*Set its size*/
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
  lv_label_set_text(label, "+");     /*Set the labels text*/
  lv_obj_center(label);

  lv_obj_t *btn2 = lv_btn_create(lv_scr_act());                /*Add a button the current screen*/
  lv_obj_set_pos(btn2, 150, ROW2);                                /*Set its position*/
  lv_obj_set_size(btn2, BUTTON_SIZE, BUTTON_SIZE);                              /*Set its size*/
  lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label2 = lv_label_create(btn2); /*Add a label to the button*/
  lv_label_set_text(label2, "-");     /*Set the labels text*/
  lv_obj_center(label2);
}

void setupBackground()
{
  static lv_style_t style_base;
  lv_style_init(&style_base);
  lv_style_set_radius(&style_base, 10);
  lv_style_set_bg_opa(&style_base, LV_OPA_COVER);
  lv_style_set_bg_color(&style_base, lv_palette_main(LV_PALETTE_NONE));

  lv_obj_t *obj = lv_obj_create(lv_scr_act());
  lv_obj_add_style(obj, &style_base, 0);
  lv_obj_center(obj);
  lv_obj_set_size(obj, 243, 243);
}

void setupLabel_Row1()
{
  static lv_style_t style_base;
  lv_style_init(&style_base);
  lv_style_set_radius(&style_base, 10);
  lv_style_set_bg_opa(&style_base, LV_OPA_COVER);
  lv_style_set_bg_color(&style_base, lv_color_white());

  lv_obj_t *la = lv_label_create(lv_scr_act());
  lv_obj_add_style(la, &style_base, 0);
  lv_obj_set_pos(la, 10, ROW1-20); 
  lv_label_set_text(la, "Poids:");

  lv_obj_t *tb = lv_textarea_create(lv_scr_act());
  lv_obj_add_style(tb, &style_base, 0);
  lv_obj_set_pos(tb, 10, ROW1);  
  lv_obj_set_size(tb, 125, BUTTON_SIZE);
}

void setupLabel_Row2()
{
  static lv_style_t style_base;
  lv_style_init(&style_base);
  lv_style_set_radius(&style_base, 10);
  lv_style_set_bg_opa(&style_base, LV_OPA_COVER);
  lv_style_set_bg_color(&style_base, lv_color_white());

  lv_obj_t *la = lv_label_create(lv_scr_act());
  lv_obj_add_style(la, &style_base, 0);
  lv_obj_set_pos(la, 10, ROW2-20); 
  lv_label_set_text(la, "Grandeur:");

  lv_obj_t *tb = lv_textarea_create(lv_scr_act());
  lv_obj_add_style(tb, &style_base, 0);
  lv_obj_set_pos(tb, 10, ROW2);  
  lv_obj_set_size(tb, 125, BUTTON_SIZE);
}

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);

  wifiserver = WifiServer::GetInstance();
  wifiserver->Initialise();  

  watch.begin();

  beginLvglHelper();
  setupBackground();
  setupLabel_Row1();
  setupLabel_Row2();
  makeButtons_Row1();
  makeButtons_Row2();
}

bool test = true;
int compteur = 0;

void loop() {

  unsigned long currentMillis = millis();
  lv_task_handler();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    compteur++;
    Serial.print(compteur);
    Serial.println("   1 second has pass");
  }

  WifiServer::upDate();
}

// void TestSendingtoExo()
// {
//   MessageBuilder message = MessageBuilder();
//   unsigned char confirmation[32] = "Hello this is the first message";
//   message.add("Hello this is the first message");
//   message.add(EnumBnoPosition::THIGH_L, 42.2);
//   Serial.println("Going to build the message");
//   int length = message.buildMessage();
//   Serial.println("Build message finished");
//   IPAddress address;
//   wifiserver->retrieveInformation(EnumIPType::EXOSKELETON, &address);
//   wifiserver->SendData(message.getMessage(), length, address);
//   Serial.print("Message to: ");
//   Serial.println(address.toString());
//   unsigned char *mess = message.getMessage();
//   for(int i = 0; i < length; i++)
//   {
//     Serial.print((char)mess[i]);
//   }
//   Serial.println("Message Sent");
// }

// void TestDeserializeData()
// {
//   MessageBuilder message = MessageBuilder();
//   unsigned char confirmation[16] = "this is my logs";
//   message.add("this is my logs");
//   message.add(EnumBnoAngle::EXO_BACK, 42.5);
//   message.add(EnumBnoPosition::THIGH_L, 78.5);
//   Serial.println("Going to build the message");
//   int length = message.buildMessage();
//   Serial.println("Build message finished");
//   wifiserver->deserializeMessage(message.getMessage(), length);
//   float exoBack = 0;

//   if(wifiserver->retrieveInformation(EnumBnoAngle::EXO_BACK, &exoBack) >= 0)
//   {
//     Serial.print("exoback: ");
//     Serial.println(exoBack);
//   }
//   else
//   {
//     Serial.println("No value found for EXO_BACK");
//   }

//   if(wifiserver->retrieveInformation(EnumBnoAngle::HIP_L, &exoBack) >= 0)
//   {
//     Serial.print("HIP_L: ");
//     Serial.println(exoBack);
//   }
//   else
//   {
//     Serial.println("No value found for HIP_L");
//   }

//   Serial.println("\n\n\n\ngoing to make the IP adresse");

//   MessageBuilder message2 = MessageBuilder();
//   IPAddress ipAdd(192, 168, 4, 3);
//   message2.add(EnumIPType::EXOSKELETON, &ipAdd);
//   Serial.println("Going to build the message");
//   length = message2.buildHandshake();
//   Serial.println("Build message finished");
//   wifiserver->deserializeMessage(message2.getMessage(), length);
//   Serial.print("Finished");
// }