///**G�re une horloge temps r�elle qui permet de dater les logs si elle est mise � l'heure gr�ce au t�l�phone

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
