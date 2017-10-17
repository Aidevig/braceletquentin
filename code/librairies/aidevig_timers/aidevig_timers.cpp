#include <aidevig_timers.h>

void setTimer(){
   // Set up the generic clock (GCLK4) used to clock timers
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC4_COUNT8_CC0 = 0x64;                      // Set the TC4 CC0 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_CC1 = 0xFE;                      // Set the TC4 CC1 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_PER = 0x1E;                      // Set the PER (period) register to its maximum value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC4_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts

  /*REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;*/               // Enable TC4
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC5_CTRLA |= TC_CTRLA_MODE_COUNT16;           // Set the counter to 8-bit mode
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC5_COUNT16_CC0 = 0xFFFF;                      // Set the TC4 CC0 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC5_COUNT16_CC1 = 0xFE;                      // Set the TC4 CC1 register to some arbitary value
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
//  REG_TC5_COUNT16_PER = 0x0D;                      // Set the PER (period) register to its maximum value
//  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC5_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  REG_TC5_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC5_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts

  /*REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;*/               // Enable TC4
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}



void tcDisable()
{
  TC4->COUNT8.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}

bool tcIsSyncing()
{
  return TC4->COUNT8.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void tcStartCounter()
{
  TC4->COUNT8.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}


void tc5Disable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tc5IsSyncing());
}

bool tc5IsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void tc5StartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_ENABLE; //set the CTRLA register
  while (tc5IsSyncing()); //wait until snyc'd
}
