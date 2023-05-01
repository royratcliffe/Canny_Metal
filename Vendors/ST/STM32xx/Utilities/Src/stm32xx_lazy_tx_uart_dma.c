// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_lazy_tx_uart_dma.h"

#include "stm32xx_registered_uart.h"
#include "stm32xx_uart_cb.h"

#include "FreeRTOS.h"
#include "task.h"

#include "list_ex.h"

static LazyTxHandle_t xLazyTxForUART[stm32xx_uartMAX_INSTANCES];
static ListItem_t *pxTxCpltForUART[stm32xx_uartMAX_INSTANCES];

LazyTxHandle_t xLazyTxForUARTDMA(UARTHandle_t xUART) { return xLazyTxForUART[xRegisteredCardinalOfUART(xUART)]; }

/*!
 * Looks for success. Delays for a small amount of time on failure, including
 * busy status.
 */
static void prvLazyTxHandler(void *pvTransmitter, const void *pvTxData, size_t xDataLengthBytes) {
  while (HAL_UART_Transmit_DMA(pvTransmitter, pvTxData, xDataLengthBytes) != HAL_OK)
    vTaskDelay(lazytx_uartdmaDELAY_TICKS);
  (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

static void prvTxCplt(UARTHandle_t xUART) {
  LazyTxHandle_t xLazyTx = xLazyTxForUARTDMA(xUART);
  if (xLazyTx) vTaskNotifyGiveFromISR(xLazyTxTaskHandle(xLazyTx), NULL);
}

LazyTxHandle_t xLazyTxForUARTDMACreate(UARTHandle_t xUART) {
  size_t xCardinal = xRegisteredCardinalOfUART(xUART);
  LazyTxHandle_t xLazyTx = xLazyTxCreate(xUART, prvLazyTxHandler);
  xLazyTxForUART[xCardinal] = xLazyTx;
  /*
   * Capturing the task handle is an alternative approach. This requires C11
   * standard. No straightforward alternative allows for a mapping between a
   * UART handle and its task. Trouble is, this requires that the system has the
   * ability to execute RAM-based thunks. Under normal circumstances, when
   * firmware cannot execute RAM, attempts to bounce via thunks will trigger a
   * fault.
   *
   *  void prvTxComplete(UART_HandleTypeDef *hUART)
   *  {
   *    vTaskNotifyGiveFromISR(xTask, NULL);
   *  }
   */
  pxTxCpltForUART[xCardinal] = pxUARTRegisterTxCplt(xUART, prvTxCplt, portMAX_DELAY);
  return xLazyTx;
}
