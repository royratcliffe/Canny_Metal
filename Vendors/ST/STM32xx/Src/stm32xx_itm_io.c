/*!
 * \file stm32xx_itm_io.c
 *
 * Connects the \c newlib low-level input-output system with the STM32.
 *
 * Do not forget to enable the Serial-Wire Viewer (SWV) with the correct core
 * clock frequency. Enable ITM Stimulus Port 0 and Start Trace. Output will
 * appear in the SWV's ITM Data Console under Port 0.
 */

#include "stm32xx_mcu.h"

int __io_putchar(int ch) {
  (void)ITM_SendChar(ch);
  return 0;
}
