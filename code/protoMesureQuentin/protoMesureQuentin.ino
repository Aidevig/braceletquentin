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
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include <SPI.h>
#include <SD.h>
#include <timers.h>
#include <ADXL345.h>
#include <ADC.h>
#include <RTC.h>
#include <pinDefinition.h>
#include <WAVrecorder.h>
#include <Wire.h>
#include <utilitaire.h>
#include <BH1790GLC.h>
extern "C" {
#include <hr_bh1790.h>
}
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif



Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


void setup() {
  
  Serial.begin(9600);
  if(DEBUG) wait5sec();
  startBluetooth();
  /// A changer lorsque l'appli pourra envoyer l'heure et la date lors de l'appairage
  setTimestamp(2017,0,0,0,0);
  adxlInit();
  setTimer();
  initHeader();
  pinConfig();
  sdStart();
  cardioInit();
  adc_init();
}

void loop() {
  ///GESTION DE L'APPUI SUR LE BOUTON
  int btn1 = managebtn(BTN_1);
  //SI APPUI LONG, ON LANCE L'ENREGISTREMENT AUDIO
  if(btn1 ==START_RECORD){
    if(DEBUG) Serial.println("Enregistrement...");
    startRec(); 
  }
  ///ARRET DE L'ENREGISTREMENT LORSQUE BOUTON RELACHE
  if(btn1 ==STOP_RECORD){
    stopRec();
    if(DEBUG) Serial.println("Fin d'enregistrement");
  }
  ///ENVOIS d'UNE ALERTE SI UNE PRESSION SUR LE BOUTON
  if(btn1 == SEND_ALERT) sendAlert();
  
  if(!record){
   ///*******GESTION COMMUNICATION USB
    if(Serial.available()>0){
        if(waitCommand()=='A'){
          sendLast();
        }
        if(waitCommand()=='G'){
          sendAudio();
        }
      }
      
  ///*************Gestion de l'accéléromètre    
  adxl.readAccel(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  //read interrupts source and look for triggerd actions
  if(digitalRead(ACCELEROMETRE_IT)){
    adxlInterrupts();
  }
  ///**********GESTION Capteur cardiaque
  resetDataCardio();
  ///FLAG 32HZ
  if (timer_flg) {
    ret16 = hr_bh1790_Calc(s_cnt_freq);
    if (ret16 == ERROR_NONE) {
      s_cnt_freq++;
      ///tourne chaque seconde
      if (s_cnt_freq >= SAMPLING_CNT_32HZ) {
        s_cnt_freq = 0;
        hr_bh1790_GetData(&bpm, &wearing);
        if(DEBUG) displayLog();
        ///ENREGISTRE LES DONNEES SUR LA CARTE SD
        refreshTimestamp();
        saveLog();
        }
      } else {
        if(DEBUG){ Serial.println(F("Error: hr_bh1790_Calc function"));Serial.print(F("ret16 = "));Serial.println(ret16, HEX);}
      }
      timer_flg = false;
    }
  }
  ///Ecris les buffer audio lorsqu'ils sont plein
  writeWav();
}

