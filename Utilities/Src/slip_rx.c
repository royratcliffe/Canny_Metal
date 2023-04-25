/*
 * slip_rx.c
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sublicense, and/or sell  copies  of   the  Software,  and to
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

#include "slip_rx.h"

#include "task.h"
#include "stream_buffer.h"

#include <string.h>

struct SLIPRx {
  StreamBufferHandle_t xStreamBuffer;
  void *pvSender;
  SLIPRxSendHandler_t xHandler;
  TaskHandle_t xTask;
};

/*!
 * \brief Receives a stream of bytes.
 *
 * Sends packets after SLIP protocol decoding. Parameters include: a receiver
 * stream buffer handle for input and a message buffer for output.
 *
 * Acts as a binding between a packet buffer stored on a task's stack, and the
 * behaviour to decode packets represented as a task.
 *
 * Automatically trims packets if their length exceeds the maximum length.
 */
static portTASK_FUNCTION(prvSLIPRxCode, pvParameters) {
  SLIPRxHandle_t xSLIPRx = pvParameters;
  uint8_t ucByte;
  uint8_t ucData[sliprxMAX_PACKET_LEN];
  size_t xReceivedBytes = 0UL;
  for (;;) {
    xStreamBufferReceive(xSLIPRx->xStreamBuffer, &ucByte, sizeof(ucByte), portMAX_DELAY);
    switch (ucByte) {
    case SLIP_END:
      if (xReceivedBytes > 0UL) {
        vSLIPRxSend(xSLIPRx, ucData, xReceivedBytes);
        xReceivedBytes = 0UL;
      }
      break;
    case SLIP_ESC:
      xStreamBufferReceive(xSLIPRx->xStreamBuffer, &ucByte, sizeof(ucByte), portMAX_DELAY);
      switch (ucByte) {
      case SLIP_ESC_END:
        ucByte = SLIP_END;
        break;
      case SLIP_ESC_ESC:
        ucByte = SLIP_ESC;
      }
    default:
      if (xReceivedBytes < sliprxMAX_PACKET_LEN) ucData[xReceivedBytes++] = ucByte;
    }
  }
}

SLIPRxHandle_t xSLIPRxCreate(size_t xBufferSizeBytes, size_t xTriggerLevelBytes) {
  SLIPRxHandle_t xSLIPRx = pvPortMalloc(sizeof(*xSLIPRx));
  configASSERT(xSLIPRx != NULL);
  (void)memset(xSLIPRx, 0, sizeof(*xSLIPRx));
  xSLIPRx->xStreamBuffer = xStreamBufferCreate(xBufferSizeBytes, xTriggerLevelBytes);
  configASSERT(xSLIPRx->xStreamBuffer);
  xTaskCreate(prvSLIPRxCode, "SLIPrx", sliprxSTACK_DEPTH, xSLIPRx, sliprxPRIORITY, &xSLIPRx->xTask);
  configASSERT(xSLIPRx->xTask != NULL);
  return xSLIPRx;
}

void vSLIPRxSendHandler(SLIPRxHandle_t xSLIPRx, void *pvSender, SLIPRxSendHandler_t xHandler) {
  xSLIPRx->pvSender = pvSender;
  xSLIPRx->xHandler = xHandler;
}

void vSLIPRxSend(SLIPRxHandle_t xSLIPRx, const void *pvTxData, size_t xDataLengthBytes) {
  if (xSLIPRx->xHandler && xDataLengthBytes) xSLIPRx->xHandler(xSLIPRx->pvSender, pvTxData, xDataLengthBytes);
}

void vSLIPRxDelete(SLIPRxHandle_t xSLIPRx) {
  vTaskDelete(xSLIPRx->xTask);
  vStreamBufferDelete(xSLIPRx->xStreamBuffer);
  vPortFree(xSLIPRx);
}
