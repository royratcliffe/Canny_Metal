/*
 * stm32xx_registered_uart.c
 */

#include "stm32xx_registered_uart.h"

#ifdef HAL_UART_MODULE_ENABLED

#include "registered_opaques.h"

static size_t prvHashOfOpaque(void *pvOpaque) {
  UART_HandleTypeDef *pUART = pvOpaque;
  return (uint32_t)pUART->Instance >> 10U;
}

size_t xRegisteredCardinalOfUART(UART_HandleTypeDef *pxUART) {
  static void *pvOpaques[stm32xx_uartMAX_INSTANCES];
  static struct RegisteredOpaques prvRegisteredOpaques = {.ppvOpaques = pvOpaques,
                                                          .xNumberOfOpaques = stm32xx_uartMAX_INSTANCES,
                                                          .pxHashOfOpaqueFunction = prvHashOfOpaque};
  return xRegisteredCardinalOfOpaque(&prvRegisteredOpaques, pxUART);
}

#endif
