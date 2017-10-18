#include <aidevig_RTC.h>

uint16_t year;
uint8_t months;
uint8_t days;
uint8_t minutes;
uint8_t seconds;
uint8_t hours;
String Timestamp;



void refreshTimestamp(){
  seconds++;
  if(seconds==60){
    seconds=0;
    minutes++;
    if(minutes==60){
      minutes=0;
      hours++;
      if(hours==24){
        hours=0;
        days++;
        switch(months){
          case 1:
          case 3:
          case 5:
          case 7:
          case 8:
          case 10:
          case 12: if(days==32){
                      days=0;
                      months++;
                      if(months==13){
                        months=0;
                      }
                  }
                   break;
          case 2: if(days==29){
                    days=0;
                    months++;
                  }
                  break;
          default: if(days==31){
                      days=0;
                      months++;
                  }
                   break;
        }
      }
    }
  }
}

void printTimestamp(File toWrite){
  toWrite.print(days);
  toWrite.print("/");
  toWrite.print(months);
  toWrite.print("/");
  toWrite.print(year);
  toWrite.print(" ");
  toWrite.print(hours);
  toWrite.print(":");
  toWrite.print(minutes);
  toWrite.print(":");
  toWrite.println(seconds);
}

void displayTimestamp(){
  Serial.print(days);
  Serial.print("/");
  Serial.print(months);
  Serial.print("/2017 ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);
  Serial.println();
}

void setTimestamp(uint16_t year_ref, uint8_t months_ref, uint8_t days_ref, uint8_t hours_ref, uint8_t seconds_ref){
    year = year_ref;
    months = months_ref;
    days = days_ref;
    hours = hours_ref;
    seconds = seconds_ref;
}
