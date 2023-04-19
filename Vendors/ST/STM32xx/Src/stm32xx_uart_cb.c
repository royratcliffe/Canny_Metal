/*
 * stm32xx_uart_cb.c
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "stm32xx_uart_cb.h"

#include "stm32xx_registered_uart.h"

#include "list_ex.h"

static List_t xRxEventForUART[stm32xx_uartMAX_INSTANCES];

static void __attribute__((constructor)) prvListInitialise() {
  for (size_t xCardinal = 0U; xCardinal < stm32xx_uartMAX_INSTANCES; xCardinal++) {
    vListInitialise(xRxEventForUART + xCardinal);
  }
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer)
#else
void HAL_UARTEx_RxEventCallback(UARTHandle_t xUART, uint16_t usXfer)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTRxEventHandler_t)pxOwner)(xUART, usXfer);
    return pdPASS;
  }
  vListYield(xRxEventForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterRxEvent(UARTHandle_t xUART, UARTRxEventHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xRxEventForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterRxEventCallback(xUART, prvRxEvent);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}
