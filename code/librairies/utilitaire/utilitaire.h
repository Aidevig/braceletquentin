#ifndef UTILITAIRE_H_INCLUDED
#define UTILITAIRE_H_INCLUDED
#include <arduino.h>
#include <ADXL345.h>
#include <timers.h>
#include <SD.h>
#include <Wire.h>
#include <ADC.h>
#include <RTC.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include <pinDefinition.h>
#include <BH1790GLC.h>
extern "C" {
#include <hr_bh1790.h>
}
#define DEBUG 1


///Macros appui bouton
#define START_RECORD 100
#define STOP_RECORD 101
#define SEND_ALERT 1

#define SAMPLING_CNT_32HZ   (32)

extern Adafruit_BluefruitLE_SPI ble;

///*****Variables global pour la gestion du bouton*****
extern int debouncetime;
extern int debouncetimer;
extern int timerbtnpressed;
extern int timelongpress;
extern int timeshortpress;
extern bool record;
extern bool shortime;
extern int shorttimer;
extern bool btnactive;
extern int btncount;

extern unsigned long recByteCount;
extern byte bufWrite;
extern byte buf00[512]; // buffer array 1
extern byte buf01[512]; // buffer array 2
extern int test;
extern int start[512];
extern unsigned int bufByteCount;


extern uint8_t s_cnt_freq;
extern BH1790GLC bh1790glc;
extern uint16_t ret16;
extern uint8_t  bpm;
extern uint8_t  wearing;


extern ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
extern int x;
extern int y;
extern int z;

extern bool timer_flg;

void startBluetooth();

void sdStart();

void sendAlert();
void displayLog();
void saveLog();

int managebtn(int pin);
void pinConfig();
void wait5sec();
void blinkFreeze(int pin);
void blinkFast(int pin);

void cardioInit();
void resetDataCardio();
int8_t bh1790_Read(uint8_t adr, uint8_t *data, uint8_t size);
int8_t bh1790_Write(uint8_t adr, uint8_t *data, uint8_t size);

void adxlInit();
void adxlInterrupts();

void TC4_Handler();
void TC5_Handler();


#endif // UTILITAIRE_H_INCLUDED
