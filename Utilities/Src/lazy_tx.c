/*
 * lazy_tx_uart.c
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of   the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "lazy_tx.h"

struct LazyTx {
  StreamBufferHandle_t xStreamBuffer;
  TxHandler_t Handler;
  void *pvSender;
  TaskHandle_t xTask;
};

/*!
 * Receives data from a stream buffer. Writes the transmit data to the HAL
 * interrupt interface, or some other node in a transmitter chain.
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
    if (xBufferLengthBytes > 0UL) { (*xLazyTx->Handler)(xLazyTx->pvSender, ucTxData, xBufferLengthBytes); }
  }
}

LazyTxHandle_t xLazyTxCreate(void *pvSender, TxHandler_t Handler) {
  LazyTxHandle_t xLazyTx = pvPortMalloc(sizeof(*xLazyTx));
  configASSERT(xLazyTx != NULL);
  xLazyTx->xStreamBuffer = xStreamBufferCreate(lazytxBUFFER_SIZE_BYTES, lazytxTRIGGER_LEVEL_BYTES);
  xLazyTx->pvSender = pvSender;
  xLazyTx->Handler = Handler;
  xTaskCreate(prvLazyTxTask, "lazytx", configMINIMAL_STACK_SIZE + lazytxBUFFER_LENGTH_BYTES, xLazyTx, lazytxPRIORITY,
              &xLazyTx->xTask);
  configASSERT(xLazyTx->xTask != NULL);
  return xLazyTx;
}

TaskHandle_t xLazyTxTaskHandle(LazyTxHandle_t xLazyTx) { return xLazyTx->xTask; }

size_t xLazyTxSend(LazyTxHandle_t xLazyTx, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait) {
  return xStreamBufferSend(xLazyTx->xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait);
}
