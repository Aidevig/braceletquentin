///**G�re l'enregistrement de fichiers WAV et l'envois des fichiers en USB

#ifndef WAVRECORDER_H_INCLUDED
#define WAVRECORDER_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <aidevig_timers.h>

extern File toSend;
extern File rec;
extern bool record;

extern int count;
extern unsigned int bufByteCount;
extern unsigned long recByteCount;
extern unsigned long recByteSaved;
extern byte buf00[512]; // buffer array 1
extern byte buf01[512]; // buffer array 2


///Initialise les variables pour l'�criture du header WAV
void initHeader();

///Stop l'enregistrement et fini l'�criture du hearder WAV
void stopRec();

///Ecrit le header WAV puis Demarre le timer TC4 pour g�n�rer les IT 8Khz
void startRec();

///Ecrite le header WAV
void writeOutHeader();

///Termine le header WAV en ajoutant la longueur du fichier
void writeWavHeader();

///Compte les enregistrement audios pr�sents sur la carte
int countRec();

///Vide le contenu du buffer audio sur le carte SD
void writeWav();

///Compte les enregistrements audio et Renvois le nom "wavXX.wav" correspondant au nombre d�j� pr�sent + 1
String checkName();

///FONCTIONS DE COMMUNICATION USB
///renvois la commande de l'ordinateur
char waitCommand();
///envois de tous les fichiers audio sur l'ordinateur
void sendAudio();
///envois du dernier fichiers audio sur l'ordinateur
void sendLast();


#endif // WAVRECORDER_H_INCLUDED
