#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <Arduino.h>
#include <SD.h>

extern uint16_t year;
extern uint8_t months;
extern uint8_t days;
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t hours;
extern String Timestamp;

void refreshTimestamp();
void printTimestamp(File toWrite);
void displayTimestamp();
void setTimestamp(uint16_t year_ref, uint8_t months_ref, uint8_t days_ref, uint8_t hours_ref, uint8_t seconds_ref);

#endif // RTC_H_INCLUDED
