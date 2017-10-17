///**Gère l'enregistrement de fichiers WAV et l'envois des fichiers en USB
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


///Initialise les variables pour l'écriture du header WAV
void initHeader();

///Stop l'enregistrement et fini l'écriture du hearder WAV
void stopRec();

///Ecrit le header WAV puis Demarre le timer TC4 pour générer les IT 8Khz
void startRec();

///Ecrite le header WAV
void writeOutHeader();

///Termine le header WAV en ajoutant la longueur du fichier
void writeWavHeader();

///Compte les enregistrement audios présents sur la carte
int countRec();

///Vide le contenu du buffer audio sur le carte SD
void writeWav();

///Compte les enregistrements audio et Renvois le nom "wavXX.wav" correspondant au nombre déjà présent + 1
String checkName();

///FONCTIONS DE COMMUNICATION USB
///renvois la commande de l'ordinateur
char waitCommand();
///envois de tous les fichiers audio sur l'ordinateur
void sendAudio();
///envois du dernier fichiers audio sur l'ordinateur
void sendLast();

///ENvois les Logs sur l'ordinateur
void sendLog();


#endif // WAVRECORDER_H_INCLUDED
