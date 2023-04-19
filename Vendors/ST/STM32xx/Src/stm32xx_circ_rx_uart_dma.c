/*
 * \file stm32xx_circ_rx_uart_dma.c
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

#include "stm32xx_circ_rx_uart_dma.h"

#include "stm32xx_registered_uart.h"
#include "stm32xx_uart_cb.h"

#include "list_ex.h"

static CircRxHandle_t xCircRxForUART[stm32xx_uartMAX_INSTANCES];

CircRxHandle_t xCircRxForUARTDMA(UARTHandle_t xUART) { return xCircRxForUART[xRegisteredCardinalOfUART(xUART)]; }

UARTHandle_t xUARTDMAForCircRx(CircRxHandle_t xCircRx) {
  for (size_t xCardinal = 0UL; xCardinal < stm32xx_uartMAX_INSTANCES; xCardinal++)
    if (xCircRxForUART[xCardinal] == xCircRx) return xUARTOfRegisteredCardinal(xCardinal);
  return NULL;
}

static portTASK_FUNCTION(prvCircRxUARTDMATask, pvParameters) {
  CircRxHandle_t xCircRx = pvParameters;
  UARTHandle_t xUART = xUARTDMAForCircRx(xCircRx);
  uint8_t ucBuffer[circrx_uartdmaBUFFER_LENGTH_BYTES];
  while (HAL_UARTEx_ReceiveToIdle_DMA(xUART, ucBuffer, sizeof(ucBuffer)) != HAL_OK)
    vTaskDelay(circrx_uartdmaDELAY_TICKS);
  vCircRx(xCircRx, ucBuffer, sizeof(ucBuffer));
}

/*
 * The receive event handler requires the circular receiver's task handle.
 * Access to the handle requires, in turn, an association between the event UART
 * and the task. Hence an extra indirection becomes necessary: from UART to
 * event handler then from UART to circular receiver.
 */
static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer) {
  CircRxHandle_t xCircRx = xCircRxForUARTDMA(xUART);
  if (xCircRx) vCircRxNotifyFromISR(xCircRx, usXfer, NULL);
}

CircRxHandle_t xCircRxUARTDMACreate(UARTHandle_t xUART, void *pvSender, TxHandler_t xHandler) {
  CircRxHandle_t xCircRx = xCircRxCreate(pvSender, xHandler);
  size_t xCardinal = xRegisteredCardinalOfUART(xUART);
  xCircRxForUART[xCardinal] = xCircRx;
  TaskHandle_t xTask;
  xTaskCreate(prvCircRxUARTDMATask, "circrxUARTDMA", circrx_uartdmaSTACK_DEPTH, xCircRx, circrx_uartdmaPRIORITY,
              &xTask);
  configASSERT(xTask != NULL);
  vCircRxTaskHandle(xCircRx, xTask);
  pxRxEventForUART[xCardinal] = pxUARTRegisterRxEvent(xUART, prvRxEvent, portMAX_DELAY);
  return xCircRx;
}

void vCircRxUARTDMADelete(UARTHandle_t xUART) {
  size_t xCardinal = xRegisteredCardinalOfUART(xUART);
  vUARTUnregisterRxEvent(pxRxEventForUART[xCardinal]);
  pxRxEventForUART[xCardinal] = NULL;
  vCircRxDelete(xCircRxForUART[xCardinal]);
  xCircRxForUART[xCardinal] = NULL;
}
