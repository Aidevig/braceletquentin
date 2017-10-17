///**Gère l'initialisation, la mise en route et l'arrêt des timers
///**Initialise le timer TC4 pour IT sur overflow à 8Khz
///**Initialise le timer TC5 pour IT sur overflow à 32Hz

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
