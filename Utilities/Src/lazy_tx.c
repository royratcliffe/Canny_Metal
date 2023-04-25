// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "lazy_tx.h"

struct LazyTx {
  StreamBufferHandle_t xStreamBuffer;
  TxHandler_t xHandler;
  void *pvSender;
  TaskHandle_t xTask;
};

/*!
 * \brief Lazy transmitter code.
 *
 * Receives data from a stream buffer. Writes the transmit data to the HAL
 * interrupt interface, typically, or some other node in a transmitter chain.
 *
 * Three parameters determine when the stream empties: the lazy buffer length in
 * bytes, the lazy delay in ticks, but also the lazy transmit stream's trigger
 * level.
 */
static portTASK_FUNCTION(prvLazyTxTask, pvParameters) {
  LazyTxHandle_t xLazyTx = pvParameters;
  for (;;) {
    uint8_t ucTxData[lazytxBUFFER_LENGTH_BYTES];
    size_t xBufferLengthBytes =
        xStreamBufferReceive(xLazyTx->xStreamBuffer, ucTxData, sizeof(ucTxData), lazytxDELAY_TICKS);
    (*xLazyTx->xHandler)(xLazyTx->pvSender, xBufferLengthBytes ? ucTxData : NULL, xBufferLengthBytes);
  }
}

LazyTxHandle_t xLazyTxCreate(void *pvSender, TxHandler_t xHandler) {
  LazyTxHandle_t xLazyTx = pvPortMalloc(sizeof(*xLazyTx));
  configASSERT(xLazyTx != NULL);
  xLazyTx->xStreamBuffer = xStreamBufferCreate(lazytxBUFFER_SIZE_BYTES, lazytxTRIGGER_LEVEL_BYTES);
  xLazyTx->pvSender = pvSender;
  xLazyTx->xHandler = xHandler;
  xTaskCreate(prvLazyTxTask, "lazytx", lazytxSTACK_DEPTH, xLazyTx, lazytxPRIORITY, &xLazyTx->xTask);
  configASSERT(xLazyTx->xTask != NULL);
  return xLazyTx;
}

TaskHandle_t xLazyTxTaskHandle(LazyTxHandle_t xLazyTx) { return xLazyTx->xTask; }

size_t xLazyTxSend(LazyTxHandle_t xLazyTx, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait) {
  return xStreamBufferSend(xLazyTx->xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait);
}
