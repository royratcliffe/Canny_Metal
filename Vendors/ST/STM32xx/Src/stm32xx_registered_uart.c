/*
 * stm32xx_registered_uart.c
 */

#include "stm32xx_registered_uart.h"

#ifdef HAL_UART_MODULE_ENABLED

#include "registered_opaques.h"

static void *pvOpaques[stm32xx_uartMAX_INSTANCES];

static size_t prvHashOfOpaque(void *pvOpaque) {
  UARTHandle_t xUART = pvOpaque;
  return (uint32_t)xUART->Instance >> 10U;
}

size_t xRegisteredCardinalOfUART(UARTHandle_t xUART) {
  static struct RegisteredOpaques prvRegisteredOpaques = {.ppvOpaques = pvOpaques,
                                                          .xNumberOfOpaques = stm32xx_uartMAX_INSTANCES,
                                                          .pxHashOfOpaqueFunction = prvHashOfOpaque};
  return xRegisteredCardinalOfOpaque(&prvRegisteredOpaques, xUART);
}

UARTHandle_t xUARTOfRegisteredCardinal(size_t xCardinal) { return pvOpaques[xCardinal]; }

#endif
