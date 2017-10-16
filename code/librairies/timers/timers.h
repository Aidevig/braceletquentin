#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED

#include <Arduino.h>


void setTimer();
void tcDisable();
bool tcIsSyncing();
void tcStartCounter();
void tc5Disable();
bool tc5IsSyncing();
void tc5StartCounter();

#endif // TIMERS_H_INCLUDED
