#include <aidevig_ADC.h>


static void   ADCsync() {
  while (ADC->STATUS.bit.SYNCBUSY == 1); //Just wait till the ADC is free
}

//##############################################################################
// Stripped-down fast analogue read anaRead()
// ulPin is the analog input pin number to be read.
////##############################################################################
byte anaRead() {

  ADCsync();
  ADC->INPUTCTRL.bit.MUXPOS = g_APinDescription[ADCPIN].ulADCChannelNumber; // Selection for the positive ADC input

  ADCsync();
  ADC->CTRLA.bit.ENABLE = 0x01;             // Enable ADC

  ADC->INTFLAG.bit.RESRDY = 1;              // Data ready flag cleared

  ADCsync();
  ADC->SWTRIG.bit.START = 1;                // Start ADC conversion

  while ( ADC->INTFLAG.bit.RESRDY == 0 );   // Wait till conversion done
  ADCsync();
  byte valueRead = ADC->RESULT.reg;

  ADCsync();
  ADC->CTRLA.bit.ENABLE = 0x00;             // Disable the ADC
  ADCsync();
  ADC->SWTRIG.reg = 0x01;                    //  and flush for good measure
  return valueRead;
}
//##############################################################################



void adc_init(){
  //###################################################################################
  // ADC setup stuff
  //###################################################################################
  ADCsync();
  ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain select as 1X
  ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC0_Val; //  2.2297 V Supply VDDANA


  // Set sample length and averaging
  ADCsync();
  ADC->AVGCTRL.reg = 0x00 ;       //Single conversion no averaging
  ADCsync();
  ADC->SAMPCTRL.reg = 0x0A;  ; //sample length in 1/2 CLK_ADC cycles Default is 3F

  //Control B register
  //int16_t ctrlb = 0x400;       // Control register B hibyte = prescale, lobyte is resolution and mode
  int16_t ctrlb = 0x430;
  ADCsync();
  ADC->CTRLB.reg =  ctrlb     ;
  anaRead();  //Discard first conversion after setup as ref changed
}
