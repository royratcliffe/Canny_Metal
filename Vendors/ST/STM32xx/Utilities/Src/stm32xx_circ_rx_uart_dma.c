// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_circ_rx_uart_dma.h"

#include "stm32xx_registered_uart.h"
#include "stm32xx_uart_cb.h"

#include "list_ex.h"

#include <stdio.h>

static CircRxHandle_t xCircRxForUART[stm32xx_uartMAX_INSTANCES];
static ListItem_t *pxRxEventForUART[stm32xx_uartMAX_INSTANCES];

CircRxHandle_t xCircRxForUARTDMA(UARTHandle_t xUART) { return xCircRxForUART[xRegisteredCardinalOfUART(xUART)]; }

UARTHandle_t xUARTDMAForCircRx(CircRxHandle_t xCircRx) {
  for (size_t xCardinal = 0UL; xCardinal < stm32xx_uartMAX_INSTANCES; xCardinal++)
    if (xCircRxForUART[xCardinal] == xCircRx) return xUARTOfRegisteredCardinal(xCardinal);
  return NULL;
}

/*!
 * \brief Code for circular DMA-based UART receiver.
 *
 * \c HAL_UARTEx_ReceiveToIdle_DMA fails if the UART peripheral state is busy, not ready; or the receive buffer is \c NULL or has zero size; or a DMA receive fails to start. The UART channel requires a circular DMA channel linkage.
 */
static portTASK_FUNCTION(prvCircRxUARTDMACode, pvParameters) {
  CircRxHandle_t xCircRx = pvParameters;
  UARTHandle_t xUART = xUARTDMAForCircRx(xCircRx);
  uint8_t ucBuffer[circrx_uartdmaBUFFER_LENGTH_BYTES];
  if (HAL_UARTEx_ReceiveToIdle_DMA(xUART, ucBuffer, sizeof(ucBuffer)) != HAL_OK)
    vTaskDelay(circrx_uartdmaDELAY_TICKS);
  vCircRx(xCircRx, ucBuffer, sizeof(ucBuffer));
}

/*!
 * \brief Handles UART receiver event.
 *
 * The receive event handler requires the circular receiver's task handle.
 * Access to the handle requires, in turn, an association between the event UART
 * and the task. Hence an extra indirection becomes necessary: from UART to
 * event handler then from UART to circular receiver.
 *
 * The ST hardware abstraction layer's UART \c RxXferCount is a down counter. It
 * starts at \c RxXferSize and continues during transfer until it decrements to
 * zero. The receive buffer pointer \c pRxBuffPtr increments at each successful
 * transfer. The end of the transfer span of the buffer matches the buffer
 * pointer plus the transfer count. It points to the end already if the transfer
 * count is zero. The start of the original buffer corresponds to this end
 * pointer minus the original size, \c RxXferSize counter.
 *
 * \sa https://community.st.com/s/article/faq-stm32-hal-driver-api-and-callbacks
 * \sa https://community.st.com/s/article/STM32-UART-DMA-RX-TX
 */
static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer) {
  CircRxHandle_t xCircRx = xCircRxForUARTDMA(xUART);
  if (xCircRx) vCircRxNotifyFromISR(xCircRx, usXfer, NULL);
}

CircRxHandle_t xCircRxForUARTDMACreate(UARTHandle_t xUART, void *pvSender, TxHandler_t xHandler) {
  size_t xCardinal = xRegisteredCardinalOfUART(xUART);
  CircRxHandle_t xCircRx = xCircRxCreate(pvSender, xHandler);
  xCircRxForUART[xCardinal] = xCircRx;
  TaskHandle_t xTask;
  char cName[configMAX_TASK_NAME_LEN];
  snprintf(cName, sizeof(cName), "circrxUARTDMA@%zd", xCardinal);
  xTaskCreate(prvCircRxUARTDMACode, cName, circrx_uartdmaSTACK_DEPTH, xCircRx, circrx_uartdmaPRIORITY,
              &xTask);
  configASSERT(xTask != NULL);
  vCircRxTaskHandle(xCircRx, xTask);
  pxRxEventForUART[xCardinal] = pxUARTRegisterRxEvent(xUART, prvRxEvent, portMAX_DELAY);
  return xCircRx;
}

void vCircRxForUARTDMADelete(UARTHandle_t xUART) {
  size_t xCardinal = xRegisteredCardinalOfUART(xUART);
  vUARTUnregister(pxRxEventForUART[xCardinal]);
  pxRxEventForUART[xCardinal] = NULL;
  vCircRxDelete(xCircRxForUART[xCardinal]);
  xCircRxForUART[xCardinal] = NULL;
}
