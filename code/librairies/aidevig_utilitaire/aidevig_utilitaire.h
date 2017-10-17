///**Gère l'algorithmie basique s'appuyant sur les fonctions arduino
///**  +la gestion du bluetooth
///**  +la gestion de l'accéléromètre
///**  +la gestion du capteur cardiaque
///**  +la gestion des boutons et des gpios
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
#ifndef UTILITAIRE_H_INCLUDED
#define UTILITAIRE_H_INCLUDED
#include <arduino.h>
#include <ADXL345.h>
#include <aidevig_timers.h>
#include <SD.h>
#include <Wire.h>
#include <aidevig_ADC.h>
#include <aidevig_RTC.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include <aidevig_pinDefinition.h>
#include <BH1790GLC.h>
extern "C" {
#include <hr_bh1790.h>
}
#define DEBUG 1



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

///Séquence démarage du module bluetooth
void startBluetooth();

///Séquence démarrage de la carte SD
///ATTENTION: FREEZE LE PROCESSEUR EN CAS DE PROBLEME
void sdStart();

///Envois de l'alerte bluetooth
void sendAlert();

///Affichage console des logs
void displayLog();

///Enregistrement des logs sur carte SD
void saveLog();

///Gère le bouton passé en paramètre
///Renvois un entier selon la séquence:
/// -appuis long
/// -appuis court : 1 appui
///                 2 appuis
///                 3 appuis
///Macros appui bouton
#define START_RECORD 100 //appui long
#define STOP_RECORD 101  //appui long relaché
#define SEND_ALERT 1
///A définir pour 2 et 3 appuis
int managebtn(int pin);

///Routine de configuration des différents GPIOS
void pinConfig();

///Utilie pour la débug, attend que le terminal soit ouvert et affiche un compte jusqu'à 4 avant de lancer le programme
void wait5sec();

///BLOQUE LE PROGRAMME ET BLINK LA LED PASSE EN PARAMETRE POUR AVERTIR L'UTILISATEUR D'UN PROBLEME MAJEUR
void blinkFreeze(int pin);

///Bloque le programme 5 secondes pour blink rapidement la led en paramètre
void blinkFast(int pin);

///Routines d'utilisation et de lecture du capteur cardiaque
void cardioInit();
void resetDataCardio();
int8_t bh1790_Read(uint8_t adr, uint8_t *data, uint8_t size);
int8_t bh1790_Write(uint8_t adr, uint8_t *data, uint8_t size);

///Fonction d'initalisation et de mise en marche de l'accéléromètre
///initialise les seuils des interruptions
void adxlInit();
///Fonction de lecture des interruptions de l'accéléromètre
void adxlInterrupts();


///Interruption 8KHZ
void TC4_Handler();
///Interruption 32HZ
void TC5_Handler();


#endif // UTILITAIRE_H_INCLUDED
