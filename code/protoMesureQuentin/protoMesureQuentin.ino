/**************************************************************************/
/*!
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2017, Aidevig
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/


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

///////////////////////////////DO NOT TOUCH ANY OF THESE LIBRAIRIES/////////////////////////
///***Librairies Natives Android
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif
///***Librairies Natives Adafruit pour le fonctionnement du bluetooth
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
///***Librairies Délivrées par Rohm pour le fonctionnement du capteur cardiaque
#include <BH1790GLC.h>
extern "C" {
#include <hr_bh1790.h>
}
///*******Librairie Open Source pour l'utilisation de l'ADXL345
#include <ADXL345.h>


////////////////////////////CHANGES CAN BE MADE ON THESE LIBRAIRIES/////////////////////////////
///*****Librairies développées pour le proto mesure Aidevig
///*****Elles contiennent les fonctions qui gèrent les différents périphériques

////////////Mapping HARDWARE/////////
#include <aidevig_pinDefinition.h>


#include <aidevig_timers.h> //**Gère l'initialisation, la mise en route et l'arrêt des timers
                    //**Initialise le timer TC4 pour IT sur overflow à 8Khz
                    //**Initialise le timer TC5 pour IT sur overflow à 32Hz

#include <aidevig_ADC.h>  //**Gère l'initialisation et la mise en route du convertisseur analog numérique
                 //** Contient aussi la fonction de lecture rapide afin de pouvoir respecter les 8Khz d'échantillonage pour enregistrement audio 

#include <aidevig_RTC.h> //**Gère une horloge temps réelle qui permet de dater les logs si elle est mise à l'heure grâce au téléphone

#include <aidevig_WAVrecorder.h> //**Gère l'enregistrement de fichiers WAV et l'envois des fichiers en USB

#include <aidevig_utilitaire.h> //**Gère l'algorithmie basique s'appuyant sur les fonctions arduino
                        //**  +la gestion du bluetooth
                        //**  +la gestion de l'accéléromètre
                        //**  +la gestion du capteur cardiaque
                        //**  +la gestion des boutons et des gpios
                        // CONTIENT #DEFINE DEBUG QUI ACTIVE LA DEBUG CONSOLE



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
        if(waitCommand()=='G'){
          sendLast();
        }
        if(waitCommand()=='A'){
          sendAudio();
          sendLog();
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

