// Source : https://forums.adafruit.com/viewtopic.php?t=175744&start=30

// Talks to multiple BNO085 sensors through a TCA9548 multiplexer.
//
// Uses NED/aerospace coordinates, so my axes definitions may differ from your other projects.
// Outputs data in compact Base64 format.
//
// Uses frustrating deficient Wire I2C library. Must do time-consuming SHTP header rereads, hurting overall performance.
// Need a better I2C library that allows reading an unlimited length I2C message one byte at a time without forcing any STOP/START cycles midway, because BNO085 message length is determined by bytes within the message.
//
// To improve SDA-to-SCL setup time during clock-stretching cycles, try adding a 2.7K pullup to SDA and a 4.7K pullup to SCL (or similar values), and use short I2C wires (a few cm).
//
// I measure 700us I2C clock stretching cycles. Ouch! Hillcrest, please try harder to reduce or eliminate clock stretching.
//
// To program Adafruit Feather M4 Express 3857:  select "Adafruit Feather M4 Express (SAMD51)", COMxx, "AVRISP mkII". Beware COM port may annoyingly change itself.
// To program Adafruit Metro Mini 2590:  select "Adafruit Metro", COMxx, "AVRISP mkII". This CPU is slower try fewer reports, or increase SENSOR_US, or reduce BNOs.

#include <Wire.h>
#include <Arduino.h>

#define BNOs        1           // number of BNO08x breakouts connected via TCA9548 mux
#define pinRST      A3          // output pin to BNO RST // UNUSED FOR NOW
#define MUX_SDA     26          // SDA pin for TCA9548 mux
#define MUX_SCL     27          // SCL pin for TCA9548 mux

#define MUX_ADDR    0x70        // I2C address of TCA9548 multiplexer
#define BNO_ADDR    0x4B        // I2C address of BNO085 sensor (0x4A if SA0=0, 0x4B if SA0=1)
#define I2C_CLOCK   400000L     // I2C clock rate
#define SERIAL_BAUD 115200L     // serial port baud rate
#define SENSOR_US   10000L      // time between sensor reports, microseconds, 10000L is 100 Hz, 20000L is 50 Hz

#define DEBUG       1           // output extra info: 0 off, 1 on. Beware this causes too much output data at low baud rates and/or high sensor rates.

// *******************************
// **  Request desired reports  **
// *******************************

#define ACC_REPORT   0x01   // accel report, see 6.5.9
#define GYRO_REPORT  0x02   // gyro report, see 6.5.13
#define MAG_REPORT   0x03   // magneto report, see 6.5.16
#define LAC_REPORT   0x04   // linear accel report, see 6.5.10
#define QUAT_REPORT  0x05   // quaternion report, see 6.5.18
#define TIME_REPORT  0xFB   // time report, see 7.2.1

static void request_reports(uint8_t bno)
{
  Wire.beginTransmission(MUX_ADDR);     // select BNO
  Wire.write(1 << bno);
  Wire.endTransmission();

  // request acc reports, see 6.5.4
  static const uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, ACC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_acc, sizeof(cmd_acc));  Wire.endTransmission();

  // request gyro reports, see 6.5.4
  static const uint8_t cmd_gyro[] = {21, 0, 2, 0, 0xFD, GYRO_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_gyro, sizeof(cmd_gyro));  Wire.endTransmission();

  // request magneto reports, see 6.5.4
  static const uint8_t cmd_mag[]  = {21, 0, 2, 0, 0xFD, MAG_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_mag, sizeof(cmd_mag));  Wire.endTransmission();

  // request linear acc reports, see 6.5.4
  static const uint8_t cmd_lac[]  = {21, 0, 2, 0, 0xFD, LAC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_lac, sizeof(cmd_lac));  Wire.endTransmission();

  // request quaternion reports, see 6.5.4
  static const uint8_t cmd_quat[] = {21, 0, 2, 0, 0xFD, QUAT_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_quat, sizeof(cmd_quat));  Wire.endTransmission();

  // At 10ms rate, BNO08x outputs most reports in one burst, Gyro-Quat-Lac-Mag, however Acc is asynchronous and a few percent faster. Situation may vary with SENSOR_US and maximum sensor rates.
}

// *******************
// **  Output data  **
// *******************

int16_t iax[BNOs], iay[BNOs], iaz[BNOs];             // accel, integer
int16_t igx[BNOs], igy[BNOs], igz[BNOs];             // gyro, integer
int16_t imx[BNOs], imy[BNOs], imz[BNOs];             // magneto, integer
int16_t ilx[BNOs], ily[BNOs], ilz[BNOs];             // linear accel, integer
int16_t iqw[BNOs], iqx[BNOs], iqy[BNOs], iqz[BNOs];  // quaternion, integer

char obuf[70], *pbuf;           // ensure this output buffer is big enough for your output string!

void uart_b64(int32_t i)        // output 18-bit integer as compact 3-digit base64
{
  for (int n=12; n >= 0; n-=6)
  {
    uint8_t c = (i >> n) & 63;
    *pbuf++ = (char)(c<26 ? 'A'+c : c<52 ? 'a'-26+c : c<62 ? '0'-52+c : c==62 ? '+' : '/');
  }
}

static void output_data(uint8_t bno)
{
  float kACC = 1/9.80665/256;     // scale units for my project
  float kGYR =  180/M_PI/512;
  float kMAG =       0.01/16;
  float kLAC = 1/9.80665/256;

  pbuf = obuf;                        // pointer into output buffer
  *pbuf++ = 'k';  *pbuf++ = 'q'+bno;  // string header "kq" is BNO0, "kr" is BNO1, "ks" is BNO2, etc
  uart_b64(kACC*iax[bno]);  uart_b64(-kACC*iay[bno]);  uart_b64(-kACC*iaz[bno]);  // accel,   convert from m/sec/sec*256 to       g*131072/10.0
  uart_b64(kGYR*igx[bno]);  uart_b64(-kGYR*igy[bno]);  uart_b64(-kGYR*igz[bno]);  // gyro,    convert from   rad/sec*512 to deg/sec*131072/4000.0
  uart_b64(kMAG*imx[bno]);  uart_b64(-kMAG*imy[bno]);  uart_b64(-kMAG*imz[bno]);  // magneto, convert from        uT*16  to   gauss*131072/1.0
  uart_b64(kLAC*ilx[bno]);  uart_b64(-kLAC*ily[bno]);  uart_b64(-kLAC*ilz[bno]);  // linacc,  convert from m/sec/sec*256 to       g*131072/10.0
  // my BNO08x orientation dot is towards left rear, rotate BNO08x quaternion to NED conventions
  uart_b64(iqw[bno]+iqz[bno]); uart_b64(iqx[bno]+iqy[bno]); uart_b64(iqx[bno]-iqy[bno]); uart_b64(iqw[bno]-iqz[bno]);  // quat,    no conversion required becasue it'll be normalized
  uart_b64(0);          // temp,    convert from    degC*128     to  degC*131072/100.0
  uart_b64(0);          // baro,    convert from hectoPa*1048576 to  mbar*131072/2000.0
  uart_b64(0xFF);       // status,  four 2-bit codes {sys,gyr,acc,mag}
  *pbuf++ = 13;         // CR LF
  *pbuf++ = 10;
  *pbuf++ = 0;          // terminate string
  Serial.write(obuf);   // writing one long string is *much* faster than printing individual values
}

// ******************************************
// **  Check for and parse sensor reports  **
// ******************************************

#if DEBUG
  static uint8_t printbyte(uint8_t b)
  {
    Serial.print(" ");  Serial.print(b,HEX);
    return b;
  }
#else
  #define printbyte(b) (b)
#endif

static void ensure_read_available(int16_t length)  // ensure a read byte is available, if necessary reread and discard 4-byte SHTP header, then read as much length as possible
{
  if (!Wire.available())
    Wire.requestFrom(BNO_ADDR,4+length), Wire.read(), Wire.read(), Wire.read(), Wire.read();
}

static void check_report(uint8_t bno)
{
  int16_t length;
  uint8_t channel __attribute__((unused));
  uint8_t seqnum  __attribute__((unused));

  Wire.beginTransmission(MUX_ADDR);     // select BNO
  Wire.write(1 << bno);
  Wire.endTransmission();

  Wire.requestFrom(BNO_ADDR,4+1);       // read 4-byte SHTP header and first byte of cargo
  if (DEBUG) {Serial.print("SHTP");}
  length  = printbyte(Wire.read());     // length LSB
  length |= printbyte((Wire.read() & 0x7F) << 8);  // length MSB (ignore continuation flag)
  channel = printbyte(Wire.read());     // channel number
  seqnum  = printbyte(Wire.read());     // sequence number (ignore)
  length -= 4;                          // done reading SHTP Header
  if (length <= 0 || length > 1000)     // if null/bad/degenerate SHTP header
  {
    if (DEBUG) {Serial.println(" What?");}
    return;
  }
  if (DEBUG) {Serial.print(" L=");  Serial.print(length,HEX);}
  if (DEBUG) {Serial.print(" C=");  Serial.println(channel,HEX);}

  while (length)                        // while more reports in cargo
  {
    uint8_t buf[20];                    // report buffer, big enough for longest interesting report (uninteresting reports will be ignored)
    uint16_t n = 0;                     // index into report buffer

    ensure_read_available(length);

    buf[n++] = Wire.read();  // first byte of report
    length--;

    // known reports
    if (channel==3 && buf[0]==TIME_REPORT && length >= 5-1)
    {
      for (uint8_t n=1; n<5; n++)       // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      //if (DEBUG) {Serial.println(" Time");}
      continue;
    }
    if (channel==3 && buf[0]==ACC_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      iax[bno] = *(int16_t*)&buf[4];
      iay[bno] = *(int16_t*)&buf[6];
      iaz[bno] = *(int16_t*)&buf[8];
      //if (DEBUG) {Serial.println(" Acc");}
      continue;
    }
    if (channel==3 && buf[0]==GYRO_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      igx[bno] = *(int16_t*)&buf[4];
      igy[bno] = *(int16_t*)&buf[6];
      igz[bno] = *(int16_t*)&buf[8];

      Serial.print("  X-axis: ");
      Serial.print(igx[bno]);

      Serial.print("  Y-axis: ");
      Serial.print(igy[bno]);

      Serial.print("  Z-axis: ");
      Serial.print(igz[bno]);
      //if (DEBUG) {Serial.println(" Gyro");}
      continue;
    }
    if (channel==3 && buf[0]==MAG_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      imx[bno] = *(int16_t*)&buf[4];
      imy[bno] = *(int16_t*)&buf[6];
      imz[bno] = *(int16_t*)&buf[8];
      //if (DEBUG) {Serial.println(" Mag");}
      output_data(bno);                 // magneto seems to be last report of burst, so use it to trigger data output
      continue;
    }
    if (channel==3 && buf[0]==LAC_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      ilx[bno] = *(int16_t*)&buf[4];
      ily[bno] = *(int16_t*)&buf[6];
      ilz[bno] = *(int16_t*)&buf[8];
      //if (DEBUG) {Serial.println(" Lac");}
      continue;
    }
    if (channel==3 && buf[0]==QUAT_REPORT && length >= 14-1)
    {
      for (uint8_t n=1; n<14; n++)      // read remainder of report
      {
        ensure_read_available(length);
        buf[n] = Wire.read();
        length--;
      }
      iqw[bno] = *(int16_t*)&buf[10];
      iqx[bno] = *(int16_t*)&buf[4];
      iqy[bno] = *(int16_t*)&buf[6];
      iqz[bno] = *(int16_t*)&buf[8];
      //if (DEBUG) {Serial.println(" Quat");}
      continue;
    }

    // unknown reports
    while (length)                      // discard remainder of cargo (shouldn't happen very often)
    {
      ensure_read_available(length);
      printbyte(Wire.read());
      length--;
    }
    if (DEBUG) {Serial.println(" Unknown");}
    continue;
  }
  return;
}

// **********************
// **  Setup and Loop  **
// **********************

void setup()
{
  Serial.begin(SERIAL_BAUD);            // initialize serial
  Serial.println("\nRunning...");

  Wire.setPins(MUX_SDA, MUX_SCL);
  Wire.begin();                         // initialize I2C
  Wire.setClock(I2C_CLOCK);             // set I2C clock rate

  for (uint8_t bno=0; bno<BNOs; bno++)  // request desired reports
    request_reports(bno);

  for (uint8_t bno=0; bno<BNOs; bno++)  // wait until all BNOs output non-zero quaternions
    do
      check_report(bno);
    while (!iqw[bno] && !iqx[bno] && !iqy[bno] && !iqz[bno]);
}

void loop()
{
  for (uint8_t bno=0; bno<BNOs; bno++)  // check for reports from all BNOs
    check_report(bno);
}