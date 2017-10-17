#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include <Arduino.h>
#include <aidevig_pinDefinition.h>

static __inline__ void ADCsync() __attribute__((always_inline, unused));
void adc_init();
byte anaRead();

#endif // ADC_H_INCLUDED
