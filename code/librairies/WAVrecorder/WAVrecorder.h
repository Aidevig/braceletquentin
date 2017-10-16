#ifndef WAVRECORDER_H_INCLUDED
#define WAVRECORDER_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <timers.h>

extern File toSend;
extern File rec;
extern bool record;

extern int count;
extern unsigned int bufByteCount;
extern unsigned long recByteCount;
extern unsigned long recByteSaved;
extern byte buf00[512]; // buffer array 1
extern byte buf01[512]; // buffer array 2

void initHeader();
void stopRec();
void startRec();
void writeOutHeader();
void writeWavHeader();
char waitCommand();
void sendAudio();
void sendLast();
int countRec();
void writeWav();
String checkName();

#endif // WAVRECORDER_H_INCLUDED
