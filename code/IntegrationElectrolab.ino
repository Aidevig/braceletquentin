/*****************************************************************************
  HeartRate.ino

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <SPI.h>
#include <SD.h>

#include <ADXL345.h>

File myFile;

#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"

#define BLUEFRUIT_SPI_CS 8
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4


char heartrate[3];


ADXL345 adxl; //variable adxl is an instance of the ADXL345 library


Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

#include <Wire.h>
#include <BH1790GLC.h>

extern "C" {
#include <hr_bh1790.h>
}

#define SAMPLING_CNT_32HZ   (32)

BH1790GLC bh1790glc;
volatile bool timer_flg;
void timer_isr(void);

int heartcount = 0;
int moyheart=0;
int heartbeat=0;

static uint8_t    s_cnt_freq = 0;

#define ADCPIN A3

File rec;
bool Status = false;
int wri=0;
int wri2=0;
int count = 512;
const int chipSelect = 4;
unsigned long fileSize = 0L;
unsigned long waveChunk = 16;
unsigned int waveType = 1;
unsigned int numChannels = 1;
unsigned long sampleRate = 8000;
unsigned long bytesPerSec = 8000;
unsigned int blockAlign = 1;
unsigned int bitsPerSample = 8;
unsigned long dataSize = 0L;
unsigned long recByteCount = 0L;
unsigned long recByteSaved = 0L;
const int btnStart = 6;
const int btnStop = 5;
const int ledStart = 2;
const int ledStop = 3;
int recPressed = 0;
int stopPressed = 0;
unsigned long oldTime = 0L;
unsigned long newTime = 0L;
byte buf00[512]; // buffer array 1
byte buf01[512]; // buffer array 2
byte byte1, byte2, byte3, byte4;
unsigned int bufByteCount;
byte bufWrite;
int start[512];
int test;
int starttime;
int endtime;
char command,c;
String Name;

File toSend;
String pref="REC";
String suf=".WAV";
String nom;


String checkName(){
  int i;
  nom = pref + i + suf;
  while(SD.exists(nom)){
    i++;
    nom = pref + i + suf;
  }
  return nom;
}

int countRec(){
  int i=0;
  nom = pref + i + suf;
  while(SD.exists(nom)){
    i++;
    nom = pref + i + suf;
  }
  return i;
}

void sendLast(){
  int count = countRec()-1;
  char data;
  Serial.print(1);
    nom = pref + count + suf;
    toSend = SD.open(nom);
    if(waitCommand()=='N'){
      Serial.print(toSend.name());
    }
    if(waitCommand()=='O'){
      while (toSend.available()) {
        data = toSend.read(); // on stocke le caractère dans c
        Serial.write(data);
        }
    }
   toSend.close();
}

void sendAudio(){
  int count = countRec();
  int i;
  char data;
  Serial.print(count);
  for(i=0;i<count;i++){
    nom = pref + i + suf;
    toSend = SD.open(nom);
    if(waitCommand()=='N'){
      Serial.print(toSend.name());
    }
    if(waitCommand()=='O'){
      while (toSend.available()) {
        data = toSend.read(); // on stocke le caractère dans c
        Serial.write(data);
        }
    }
   toSend.close();
  }
}

char waitCommand(){
  while(Serial.available()==0);
  return (char)Serial.read();
}

void writeWavHeader() { // write out original WAV header to file
  Name = checkName();
  recByteSaved = 0;
  Serial.print(Name);
  rec = SD.open(Name, FILE_WRITE);
  delay(300);
  if(rec){
  Serial.print(rec.write("RIFF"));
  byte1 = fileSize & 0xff;
  byte2 = (fileSize >> 8) & 0xff;
  byte3 = (fileSize >> 16) & 0xff;
  byte4 = (fileSize >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.write("WAVE");
  rec.write("fmt ");
  byte1 = waveChunk & 0xff;
  byte2 = (waveChunk >> 8) & 0xff;
  byte3 = (waveChunk >> 16) & 0xff;
  byte4 = (waveChunk >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = waveType & 0xff;
  byte2 = (waveType >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2); 
  byte1 = numChannels & 0xff;
  byte2 = (numChannels >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2); 
  byte1 = sampleRate & 0xff;
  byte2 = (sampleRate >> 8) & 0xff;
  byte3 = (sampleRate >> 16) & 0xff;
  byte4 = (sampleRate >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = bytesPerSec & 0xff;
  byte2 = (bytesPerSec >> 8) & 0xff;
  byte3 = (bytesPerSec >> 16) & 0xff;
  byte4 = (bytesPerSec >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = blockAlign & 0xff;
  byte2 = (blockAlign >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2); 
  byte1 = bitsPerSample & 0xff;
  byte2 = (bitsPerSample >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2); 
  rec.write("data");
  byte1 = dataSize & 0xff;
  byte2 = (dataSize >> 8) & 0xff;
  byte3 = (dataSize >> 16) & 0xff;
  byte4 = (dataSize >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  }
  else{
    Serial.println("BUG2");
  }
}


void writeOutHeader() { // update WAV header with final filesize/datasize

  rec.seek(4);
  byte1 = recByteSaved & 0xff;
  byte2 = (recByteSaved >> 8) & 0xff;
  byte3 = (recByteSaved >> 16) & 0xff;
  byte4 = (recByteSaved >> 24) & 0xff;  
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.seek(40);
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.close();
  
}


void setTimer(){
   // Set up the generic clock (GCLK4) used to clock timers
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  
  REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization 

  REG_TC4_COUNT8_CC0 = 0x64;                      // Set the TC4 CC0 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_CC1 = 0xFE;                      // Set the TC4 CC1 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_PER = 0x1E;                      // Set the PER (period) register to its maximum value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest) 
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC) 

  REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC4_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts
  
  /*REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;*/               // Enable TC4
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC5_CTRLA |= TC_CTRLA_MODE_COUNT16;           // Set the counter to 8-bit mode
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization 

  REG_TC5_COUNT16_CC0 = 0xFFFF;                      // Set the TC4 CC0 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC5_COUNT16_CC1 = 0xFE;                      // Set the TC4 CC1 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
//  REG_TC5_COUNT16_PER = 0x0D;                      // Set the PER (period) register to its maximum value
//  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest) 
  NVIC_EnableIRQ(TC5_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC) 

  REG_TC5_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC5_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts
  
  /*REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;*/               // Enable TC4
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}


void TC4_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{     
  // Check for overflow (OVF) interrupt 
  if (TC4->COUNT8.INTFLAG.bit.OVF && TC4->COUNT8.INTENSET.bit.OVF)             
  {
    // Put your timer overflow (OVF) code here:     
    // ...
    recByteCount++; // increment sample counter
    bufByteCount++;
    if (bufByteCount == 512 && bufWrite == 0) { 
      bufByteCount = 0;
      bufWrite = 1;
    } else if (bufByteCount == 512 & bufWrite == 1) {
      bufByteCount = 0;
      bufWrite = 0;
    }
    
    if (bufWrite == 0) { buf00[bufByteCount] = anaRead(); }
    if (bufWrite == 1) { buf01[bufByteCount] = anaRead();}
   /* test=ADC->RESULT.reg;
    if (bufWrite == 0) { buf00[bufByteCount] = test>>2; }
    if (bufWrite == 1) { buf01[bufByteCount] = test>>2;}*/
    if(bufWrite == 1 && test ==false) test = true;
    if(!test) start[bufByteCount]=micros();
    REG_TC4_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag 
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC0 && TC4->COUNT8.INTENSET.bit.MC0)             
  {
    // Put your counter compare 0 (CC0) code here:
    // ...
    //buf00[0]=analogRead(A3);
    
    Serial.println("!!!!!!!!!!!!!IT!!!!!!!!!!");
  
    REG_TC4_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag 
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC1 && TC4->COUNT8.INTENSET.bit.MC1)            
  {
    // Put your counter compare 1 (CC1) code here:
    // ...
    REG_TC4_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag 
  }
}

void tcDisable()
{
  TC4->COUNT8.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}

bool tcIsSyncing()
{
  return TC4->COUNT8.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void tcStartCounter()
{
  TC4->COUNT8.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

void startRec(){
  writeWavHeader();
  starttime=micros();
  tcStartCounter();
}

void stopRec(){
  tcDisable();
  endtime= micros()-starttime;
  writeOutHeader();
}


static __inline__ void ADCsync() __attribute__((always_inline, unused));
static void   ADCsync() {
  while (ADC->STATUS.bit.SYNCBUSY == 1); //Just wait till the ADC is free
}


//##############################################################################
// Stripped-down fast analogue read anaRead()
// ulPin is the analog input pin number to be read.
////##############################################################################
byte anaRead() {

  ADCsync();
  ADC->INPUTCTRL.bit.MUXPOS = g_APinDescription[ADCPIN].ulADCChannelNumber; // Selection for the positive ADC input

  ADCsync();
  ADC->CTRLA.bit.ENABLE = 0x01;             // Enable ADC

  ADC->INTFLAG.bit.RESRDY = 1;              // Data ready flag cleared

  ADCsync();
  ADC->SWTRIG.bit.START = 1;                // Start ADC conversion

  while ( ADC->INTFLAG.bit.RESRDY == 0 );   // Wait till conversion done
  ADCsync();
  byte valueRead = ADC->RESULT.reg;

  ADCsync();
  ADC->CTRLA.bit.ENABLE = 0x00;             // Disable the ADC 
  ADCsync();
  ADC->SWTRIG.reg = 0x01;                    //  and flush for good measure
  return valueRead;
}
//##############################################################################



void adc_init(){
  //###################################################################################
  // ADC setup stuff
  //###################################################################################
  ADCsync();
  ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain select as 1X
  ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC0_Val; //  2.2297 V Supply VDDANA

  
  // Set sample length and averaging
  ADCsync();
  ADC->AVGCTRL.reg = 0x00 ;       //Single conversion no averaging
  ADCsync();
  ADC->SAMPCTRL.reg = 0x0A;  ; //sample length in 1/2 CLK_ADC cycles Default is 3F
  
  //Control B register
  //int16_t ctrlb = 0x400;       // Control register B hibyte = prescale, lobyte is resolution and mode 
  int16_t ctrlb = 0x430;
  ADCsync();
  ADC->CTRLB.reg =  ctrlb     ; 
  anaRead();  //Discard first conversion after setup as ref changed
}



/*void setTimer5(){
   // Set up the generic clock (GCLK4) used to clock timers
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(5);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(5);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK5 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  
  REG_TC5_CTRLA |= TC_CTRLA_MODE_COUNT16;           // Set the counter to 8-bit mode
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization 

  REG_TC5_COUNT16_CC0 = 0xFFFF;                      // Set the TC4 CC0 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC5_COUNT16_CC1 = 0xFE;                      // Set the TC4 CC1 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
//  REG_TC5_COUNT16_PER = 0x0D;                      // Set the PER (period) register to its maximum value
//  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest) 
  NVIC_EnableIRQ(TC5_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC) 

  REG_TC5_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC5_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts
  
  /*REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;*/               // Enable TC4
  /*while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}*/


void TC5_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{     
  // Check for overflow (OVF) interrupt 
  if (TC5->COUNT16.INTFLAG.bit.OVF && TC5->COUNT16.INTENSET.bit.OVF)             
  {
    // Put your timer overflow (OVF) code here:     
    // ...
    REG_TC5_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag 
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC5->COUNT16.INTFLAG.bit.MC0 && TC5->COUNT16.INTENSET.bit.MC0)             
  {
    timer_flg=true;
    REG_TC5_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag 
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC5->COUNT16.INTFLAG.bit.MC1 && TC5->COUNT16.INTENSET.bit.MC1)            
  {
    // Put your counter compare 1 (CC1) code here:
    // ...
    REG_TC5_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag 
  }
}

void tc5Disable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tc5IsSyncing());
}

bool tc5IsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void tc5StartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_ENABLE; //set the CTRLA register
  while (tc5IsSyncing()); //wait until snyc'd
}


void setup() {

  ble.begin();
  ble.println("AT+GAPDEVNAME=GO-SAFE");
  ble.waitForOK();
  ble.setMode(BLUEFRUIT_MODE_DATA);
  heartrate[0]='0';
  heartrate[1]='1';
  heartrate[2]='2';
  
  
   pinMode(6,OUTPUT);
   digitalWrite(6,HIGH);
   
  pinMode(10,INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5,LOW);
  uint16_t ret16 = ERROR_NONE;

  timer_flg = false;
  Serial.begin(115200);
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625μs per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  int i = 0;
  Serial.begin(9600);
  while(!Serial);
  for(i=0;i<5;i++){
    Serial.println(i);
    delay(1000);
  }
  pinMode(6,INPUT_PULLUP);
  
  pinMode(5,INPUT_PULLUP);
  
  if(!SD.begin(12)){
    Serial.println("PB SD");
    while(1);
  }
  else{
    Serial.print("SD OK");
    
  }
  
  Wire.begin();
  Wire.setClock(400000L);
  setTimer();
   tc5StartCounter();
  s_cnt_freq = 0;

  ret16 = hr_bh1790_Init();
  if (ret16 == ERROR_NONE) {
    Serial.println(F("BPM, wearing"));    
    

    ret16 = hr_bh1790_StartMeasure();
    if (ret16 != ERROR_NONE) {
      Serial.println(F("Error: hr_bh1790_StartMeasure function"));
      Serial.print(F("ret16 = "));
      Serial.println(ret16, HEX);      
    }
  } else {
    Serial.println(F("Error: hr_bh1790_Init function"));
    Serial.print(F("ret16 = "));
    Serial.println(ret16, HEX);
  }
  
  adc_init();
}

void loop() {

  //Boring accelerometer stuff   
  int x,y,z;
   int i;
  int temp;
  double moy=0.0;
  // put your main code here, to run repeatedly:
  if(!digitalRead(6)&&Status==false){
    while(!digitalRead(6));
    Status=true;
    digitalWrite(13,Status);
    Serial.println("Enregistrement...");
    startRec();
    
  }
  if(!digitalRead(5)&&Status==true){
    Status=false;
    digitalWrite(13,Status);
    stopRec();
    Serial.println("Fin d'enregistrement");
    Serial.println(wri);
    Serial.println(wri2);
    Serial.println(recByteSaved);
    Serial.println("durée : ");
    Serial.println(endtime);
    for(i=1;i<512;i++){
      temp=start[i]-start[i-1];
      Serial.print(i);Serial.print(":");Serial.println(temp);
    }
  }
  if(Status==false){
    /*if(Serial.available()>0){
        if(waitCommand()=='A'){
          sendLast();
        }
      }*/
      adxl.readAccel(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
      if(!digitalRead(5)){
        ble.write('S');
        ble.write(heartrate,3);
      }

  //Fun Stuff!    
  //read interrupts source and look for triggerd actions
  if(digitalRead(10)){
    //getInterruptSource clears all triggered actions after returning value
    //so do not call again until you need to recheck for triggered actions
     byte interrupts = adxl.getInterruptSource();
    
    // freefall
    if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
      Serial.println("freefall");
      //add code here to do when freefall is sensed
    } 
    
    //inactivity
    if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
      Serial.println("inactivity");
       //add code here to do when inactivity is sensed
    }
    
    //activity
    if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
      Serial.println("activity"); 
       //add code here to do when activity is sensed
    }
    
    //double tap
    if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
      Serial.println("double tap");
       //add code here to do when a 2X tap is sensed
    }
    
    //tap
    if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
      Serial.println("tap");
       //add code here to do when a tap is sensed
    } 
  }

  uint8_t  bpm     = 0U;
  uint8_t  wearing = 0U;
  uint16_t ret16   = ERROR_NONE;
  if (timer_flg) {
    ret16 = hr_bh1790_Calc(s_cnt_freq);
    if (ret16 == ERROR_NONE) {
      s_cnt_freq++;
      if (s_cnt_freq >= SAMPLING_CNT_32HZ) {
        s_cnt_freq = 0;
        hr_bh1790_GetData(&bpm, &wearing);
        moyheart +=bpm;
        heartcount++;
        //Serial.print("count:");Serial.println(heartcount);
        Serial.print(bpm);
        Serial.print("//");
        // Output x,y,z values - Commented out
        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.print(",");
        Serial.println(z);

        myFile = SD.open("ASSOt.txt",FILE_WRITE);
        if(!myFile){
        Serial.println("PB FILE");
        }
        else{
          myFile.print(bpm);
          myFile.print(",");
          myFile.print(x);
          myFile.print(",");
          myFile.print(y);
          myFile.print(",");
          myFile.println(z);
          myFile.close();
          /*ble.write('S');
          ble.write(heartrate,3); 
          ble.println();*/  
          }
          /*Serial.print(bpm, DEC);
          Serial.print(F(","));
          Serial.println(wearing, DEC);*/
          /*if(heartcount==5){
            heartcount=0;
            heartbeat=moyheart/5;
            Serial.println(heartbeat);
            moyheart=0;
          }*/
        }
      } else {
        Serial.println(F("Error: hr_bh1790_Calc function"));
        Serial.print(F("ret16 = "));
        Serial.println(ret16, HEX);
      }
      timer_flg = false;
    }
  }
  
  if (recByteCount % 1024 == 512 && Status == true) {wri++; wri2+=rec.write(buf00,512); recByteSaved+= 512; } // save buf01 to card
  if (recByteCount % 1024 == 0 && Status == true) { wri++; wri2+=rec.write(buf01,512); recByteSaved+= 512; } // save buf02 to card
  
  
}

void timer_isr(void) {
  timer_flg = true;
}

int8_t bh1790_Write(uint8_t adr, uint8_t *data, uint8_t size)
{
  byte   rc  = 0;
  int8_t ret = 0;
  
  rc = bh1790glc.write(adr, data, size);
  if (rc == 0) {
    ret = BH1790_RC_OK;
  } else {
    ret = BH1790_RC_I2C_ERR;
  }

  return (ret);
}

int8_t bh1790_Read(uint8_t adr, uint8_t *data, uint8_t size)
{
  byte   rc  = 0;
  int8_t ret = 0;

  rc = bh1790glc.read(adr, data, size);
  if (rc == 0) {
    ret = BH1790_RC_OK;
  } else {
    ret = BH1790_RC_I2C_ERR;
  }
  
  return (ret);
}

