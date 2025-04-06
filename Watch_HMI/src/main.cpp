/*0000000000000000000--------- DIGITAL CLOCK ---------0000000000000000000*/
// /*
//  An example digital clock using a TFT LCD screen to show the time.
//  Demonstrates use of the font printing routines. (Time updates but date does not.)

//  It uses the time of compile/upload to set the time
//  For a more accurate clock, it would be better to use the RTClib library.
//  But this is just a demo...

//  Make sure all the display driver and pin connections are correct by
//  editing the User_Setup.h file in the TFT_eSPI library folder.

//  #########################################################################
//  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
//  #########################################################################

//  Based on clock sketch by Gilchrist 6/2/2014 1.0

// A few colour codes:

// code    color
// 0x0000  Black
// 0xFFFF  White
// 0xBDF7  Light Gray
// 0x7BEF  Dark Gray
// 0xF800  Red
// 0xFFE0  Yellow
// 0xFBE0  Orange
// 0x79E0  Brown
// 0x7E0   Green
// 0x7FF   Cyan
// 0x1F    Blue
// 0xF81F  Pink

//  */

//  #include <LilyGoLib.h> // Hardware-specific library


//  #define TFT_GREY 0x5AEB
 
//  // Invoke custom library
 
//  uint32_t targetTime = 0;                    // for next 1 second timeout
 
//  static uint8_t conv2d(const char *p); // Forward declaration needed for IDE 1.6.x
 
//  uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
 
//  byte omm = 99, oss = 99;
//  byte xcolon = 0, xsecs = 0;
//  unsigned int colour = 0;
 
//  // Create a TFT object using the hardware SPI interface
//  extern LilyGoLib watch; // Invoke custom library

//  void setup(void)
//  {

//      //Serial.begin(115200);
//      watch.begin();
//      watch.setRotation(1);
//      watch.fillScreen(TFT_BLACK);
 
//      watch.setTextSize(1);
//      watch.setTextColor(TFT_YELLOW, TFT_BLACK);
 
//      targetTime = millis() + 1000;
//  }
 
//  void loop()
//  {
//      if (targetTime < millis()) {
//          // Set next update for 1 second later
//          targetTime = millis() + 1000;
 
//          // Adjust the time values by adding 1 second
//          ss++;              // Advance second
//          if (ss == 60) {    // Check for roll-over
//              ss = 0;          // Reset seconds to zero
//              omm = mm;        // Save last minute time for display update
//              mm++;            // Advance minute
//              if (mm > 59) {   // Check for roll-over
//                  mm = 0;
//                  hh++;          // Advance hour
//                  if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
//                      hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
//                  }
//              }
//          }
 
 
//          // Update digital time
//          int xpos = 0;
//          int ypos = 85; // Top left corner ot clock text, about half way down
//          int ysecs = ypos + 24;
 
//          if (omm != mm) { // Redraw hours and minutes time every minute
//              omm = mm;
//              // Draw hours and minutes
//              if (hh < 10) xpos += watch.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
//              xpos += watch.drawNumber(hh, xpos, ypos, 8);             // Draw hours
//              xcolon = xpos; // Save colon coord for later to flash on/off later
//              xpos += watch.drawChar(':', xpos, ypos - 8, 8);
//              if (mm < 10) xpos += watch.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
//              xpos += watch.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
//              xsecs = xpos; // Sae seconds 'x' position for later display updates
//          }
//          if (oss != ss) { // Redraw seconds time every second
//              oss = ss;
//              xpos = xsecs;
 
//              if (ss % 2) { // Flash the colons on/off
//                  watch.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
//                  watch.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
//                  xpos += watch.drawChar(':', xsecs, ysecs, 6); // Seconds colon
//                  watch.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
//              } else {
//                  watch.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
//                  xpos += watch.drawChar(':', xsecs, ysecs, 6); // Seconds colon
//              }
 
//              //Draw seconds
//              if (ss < 10) xpos += watch.drawChar('0', xpos, ysecs, 6); // Add leading zero
//              watch.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
//          }
//      }
//  }
 
 
//  // Function to extract numbers from compile time string
//  static uint8_t conv2d(const char *p)
//  {
//      uint8_t v = 0;
//      if ('0' <= *p && *p <= '9')
//          v = *p - '0';
//      return 10 * v + *++p - '0';
//  }
 


/*0000000000000000000--------- PONG GAME ---------0000000000000000000*/
/*
 * Pong
 * Original Code from https://github.com/rparrett/pongclock
 *
 */

// Demo only - not playable

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

void setup(void)
{

    //randomSeed(analogRead(0)*analogRead(1));

    watch.begin();

    watch.setRotation(1);

    watch.fillScreen(BLACK);
    //watch.fillScreen(GREY);

    initgame();

    watch.setTextColor(WHITE, BLACK);

}

void loop()
{
    delay(dly);

    lpaddle();
    rpaddle();

    midline();

    ball();
}


 
 