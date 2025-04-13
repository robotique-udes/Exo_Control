#include"Arduino.h"
#include"WifiServer.h"
#include "MessageBuilder/MessageBuilder.h"
#include "enums.h"

void TestDeserializeData();
void TestSendingtoExo();

WifiServer* wifiserver;

unsigned long previousMillis = 0; // Stores the last time a message was printed
const unsigned long interval = 1000; // Interval between prints (1 second)

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

#include <LilyGoLib.h> // Hardware-specific library


// Invoke custom library

int16_t h = 240;
int16_t w = 320;

int dly = 5;

int16_t paddle_h = 30;
int16_t paddle_w = 4;

int16_t lpaddle_x = 0;
int16_t rpaddle_x = w - paddle_w;

int16_t lpaddle_y = 0;
int16_t rpaddle_y = h - paddle_h;

int16_t lpaddle_d = 1;
int16_t rpaddle_d = -1;

int16_t lpaddle_ball_t = w - w / 4;
int16_t rpaddle_ball_t = w / 4;

int16_t target_y = 0;

int16_t ball_x = 2;
int16_t ball_y = 2;
int16_t oldball_x = 2;
int16_t oldball_y = 2;

int16_t ball_dx = 1;
int16_t ball_dy = 1;

int16_t ball_w = 6;
int16_t ball_h = 6;

int16_t dashline_h = 4;
int16_t dashline_w = 2;
int16_t dashline_n = h / dashline_h;
int16_t dashline_x = w / 2 - 1;
int16_t dashline_y = dashline_h / 2;

int16_t lscore = 12;
int16_t rscore = 4;

// Create a TFT object using the hardware SPI interface
extern LilyGoLib watch; // Invoke custom library

void midline()
{
    
    // If the ball is not on the line then don't redraw the line
    if ((ball_x < dashline_x - ball_w) && (ball_x > dashline_x + dashline_w)) return;
    
    watch.startWrite();
    
    // Quick way to draw a dashed line
    watch.setAddrWindow(dashline_x, 0, dashline_w, h);
    
    for (int16_t i = 0; i < dashline_n; i += 2) {
        watch.pushColor(WHITE, dashline_w * dashline_h); // push dash pixels
        watch.pushColor(BLACK, dashline_w * dashline_h); // push gap pixels
    }
    
    watch.endWrite();
}

void lpaddle()
{
    
    if (lpaddle_d == 1) {
        watch.fillRect(lpaddle_x, lpaddle_y, paddle_w, 1, BLACK);
    } else if (lpaddle_d == -1) {
        watch.fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
    }
    
    lpaddle_y = lpaddle_y + lpaddle_d;
    
    if (ball_dx == 1) lpaddle_d = 0;
    else {
        if (lpaddle_y + paddle_h / 2 == target_y) lpaddle_d = 0;
        else if (lpaddle_y + paddle_h / 2 > target_y) lpaddle_d = -1;
        else lpaddle_d = 1;
    }
    
    if (lpaddle_y + paddle_h >= h && lpaddle_d == 1) lpaddle_d = 0;
    else if (lpaddle_y <= 0 && lpaddle_d == -1) lpaddle_d = 0;
    
    watch.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void rpaddle()
{
    
    if (rpaddle_d == 1) {
        watch.fillRect(rpaddle_x, rpaddle_y, paddle_w, 1, BLACK);
    } else if (rpaddle_d == -1) {
        watch.fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
    }
    
    rpaddle_y = rpaddle_y + rpaddle_d;
    
    if (ball_dx == -1) rpaddle_d = 0;
    else {
        if (rpaddle_y + paddle_h / 2 == target_y) rpaddle_d = 0;
        else if (rpaddle_y + paddle_h / 2 > target_y) rpaddle_d = -1;
        else rpaddle_d = 1;
    }
    
    if (rpaddle_y + paddle_h >= h && rpaddle_d == 1) rpaddle_d = 0;
    else if (rpaddle_y <= 0 && rpaddle_d == -1) rpaddle_d = 0;
    
    watch.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void calc_target_y()
{
    int16_t target_x;
    int16_t reflections;
    int16_t y;
    
    if (ball_dx == 1) {
        target_x = w - ball_w;
    } else {
        target_x = -1 * (w - ball_w);
    }
    
    y = abs(target_x * (ball_dy / ball_dx) + ball_y);
    
    reflections = floor(y / h);
    
    if (reflections % 2 == 0) {
        target_y = y % h;
    } else {
        target_y = h - (y % h);
    }
}

void ball()
{
    ball_x = ball_x + ball_dx;
    ball_y = ball_y + ball_dy;
    
    if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
        ball_dx = ball_dx * -1;
        dly = random(5); // change speed of ball after paddle contact
        calc_target_y();
    } else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
        ball_dx = ball_dx * -1;
        dly = random(5); // change speed of ball after paddle contact
        calc_target_y();
    } else if ((ball_dx == 1 && ball_x >= w) || (ball_dx == -1 && ball_x + ball_w < 0)) {
        dly = 5;
    }
    
    if (ball_y > h - ball_w || ball_y < 0) {
        ball_dy = ball_dy * -1;
        ball_y += ball_dy; // Keep in bounds
    }
    
    //watch.fillRect(oldball_x, oldball_y, ball_w, ball_h, BLACK);
    watch.drawRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); // Less TFT refresh aliasing than line above for large balls
    watch.fillRect(   ball_x,    ball_y, ball_w, ball_h, WHITE);
    oldball_x = ball_x;
    oldball_y = ball_y;
}

void initgame()
{
    lpaddle_y = random(0, h - paddle_h);
    rpaddle_y = random(0, h - paddle_h);

    // ball is placed on the center of the left paddle
    ball_y = lpaddle_y + (paddle_h / 2);

    calc_target_y();

    midline();

    watch.fillRect(0, h - 26, w, 239, GREY);

    watch.setTextDatum(TC_DATUM);
    watch.setTextColor(WHITE, GREY);
    watch.drawString("TFT_eSPI example", w / 2, h - 26, 4);
}

void setup() {
  Serial.begin(115200);
  // Connect to Wifi network.
  delay(3000);

  wifiserver = WifiServer::GetInstance();
  wifiserver->Initialise();  

  watch.begin();

    watch.setRotation(1);

    watch.fillScreen(BLACK);
    //watch.fillScreen(GREY);

    initgame();

    watch.setTextColor(WHITE, BLACK);
}

bool test = true;
int compteur = 0;

void loop() {

  delay(dly);

    lpaddle();
    rpaddle();

    midline();

    ball();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    compteur++;
    Serial.print(compteur);
    Serial.println("   1 second has pass");
  }

  WifiServer::upDate();

  // if(test && compteur >5)
  // {
  //   test = false;
  //   TestDeserializeData();
  // }

  if(test && compteur > 42)
  {
    test = false;
    TestSendingtoExo();
  }

  delay(1);
}

void TestSendingtoExo()
{
  MessageBuilder message = MessageBuilder();
  unsigned char confirmation[32] = "Hello this is the first message";
  message.add("Hello this is the first message");
  message.add(EnumBnoPosition::THIGH_L, 42.2);
  Serial.println("Going to build the message");
  int length = message.buildMessage();
  Serial.println("Build message finished");
  IPAddress address;
  wifiserver->retrieveInformation(EnumIPType::EXOSKELETON, &address);
  wifiserver->SendData(message.getMessage(), length, address);
  Serial.print("Message to: ");
  Serial.println(address.toString());
  unsigned char *mess = message.getMessage();
  for(int i = 0; i < length; i++)
  {
    Serial.print((char)mess[i]);
  }
  Serial.println("Message Sent");
}

void TestDeserializeData()
{
  MessageBuilder message = MessageBuilder();
  unsigned char confirmation[16] = "this is my logs";
  message.add("this is my logs");
  message.add(EnumBnoAngle::EXO_BACK, 42.5);
  message.add(EnumBnoPosition::THIGH_L, 78.5);
  Serial.println("Going to build the message");
  int length = message.buildMessage();
  Serial.println("Build message finished");
  wifiserver->deserializeMessage(message.getMessage(), length);
  float exoBack = 0;

  if(wifiserver->retrieveInformation(EnumBnoAngle::EXO_BACK, &exoBack) >= 0)
  {
    Serial.print("exoback: ");
    Serial.println(exoBack);
  }
  else
  {
    Serial.println("No value found for EXO_BACK");
  }

  if(wifiserver->retrieveInformation(EnumBnoAngle::HIP_L, &exoBack) >= 0)
  {
    Serial.print("HIP_L: ");
    Serial.println(exoBack);
  }
  else
  {
    Serial.println("No value found for HIP_L");
  }

  Serial.println("\n\n\n\ngoing to make the IP adresse");

  MessageBuilder message2 = MessageBuilder();
  IPAddress ipAdd(192, 168, 4, 3);
  message2.add(EnumIPType::EXOSKELETON, &ipAdd);
  Serial.println("Going to build the message");
  length = message2.buildHandshake();
  Serial.println("Build message finished");
  wifiserver->deserializeMessage(message2.getMessage(), length);
  Serial.print("Finished");
}