/*
 * circ_rx.c
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
 *		The above copyright notice and this permission notice shall be
 *		included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "circ_rx.h"

#include "tx_handler.h"

#include <limits.h>

/*
 * \brief Delete notification for circular-receive task.
 *
 * The mask bit must be at position 16 or greater in order to avoid intersecting
 * with the 16-bit transfer offset.
 */
#define circ_rxDELETE_NOTIFIED (1UL << ('_' - '@'))

struct CircRx {
  TaskHandle_t xTask;
  TxHandler_t xHandler;
  void *pvSender;
};

void vCircRxSend(CircRxHandle_t xCircRx, const void *pvTxData, size_t xDataLengthBytes) {
  if (xDataLengthBytes == 0UL) return;
  if (xCircRx->xHandler == NULL) return;
  (*xCircRx->xHandler)(xCircRx->pvSender, pvTxData, xDataLengthBytes);
}

void vCircRx(CircRxHandle_t xCircRx, void *pvRxBuffer, size_t xBufferLengthBytes) {
  uint32_t ulCirc = 0UL;
  for (;;) {
    uint32_t ulXfer;
    xTaskNotifyWait(0UL, ULONG_MAX, &ulXfer, portMAX_DELAY);
    if (ulXfer & circ_rxDELETE_NOTIFIED) break;
    if (ulCirc == ulXfer) continue;
    if (ulXfer > ulCirc)
      vCircRxSend(xCircRx, (uint8_t *)pvRxBuffer + ulCirc, ulXfer - ulCirc);
    else {
      vCircRxSend(xCircRx, (uint8_t *)pvRxBuffer + ulCirc, xBufferLengthBytes - ulCirc);
      vCircRxSend(xCircRx, pvRxBuffer, ulXfer);
    }
    ulCirc = ulXfer;
  }
  vTaskDelete(NULL);
}

CircRxHandle_t xCircRxCreate(void *pvSender, TxHandler_t xHandler) {
  CircRxHandle_t xCircRx = pvPortMalloc(sizeof(*xCircRx));
  configASSERT(xCircRx);
  xCircRx->xTask = NULL;
  xCircRx->xHandler = xHandler;
  xCircRx->pvSender = pvSender;
  return xCircRx;
}

void vCircRxDelete(CircRxHandle_t xCircRx) {
  TaskHandle_t xTask = xCircRxTaskHandle(xCircRx);
  if (xTask) (void)xTaskNotify(xTask, circ_rxDELETE_NOTIFIED, eSetBits);
  vPortFree(xCircRx);
}

TaskHandle_t xCircRxTaskHandle(CircRxHandle_t xCircRx) { return xCircRx->xTask; }

void vCircRxTaskHandle(CircRxHandle_t xCircRx, TaskHandle_t xTask) { xCircRx->xTask = xTask; }

void vCircRxStreamBuffer(CircRxHandle_t xCircRx, StreamBufferHandle_t xStreamBuffer) {
  xCircRx->xHandler = vTxStreamBufferHandler;
  xCircRx->pvSender = xStreamBuffer;
}

void vCircRxNotifyFromISR(CircRxHandle_t xCircRx, uint32_t ulXfer, BaseType_t *pxWoken) {
  uint32_t ulNotified;
  TaskHandle_t xTask = xCircRxTaskHandle(xCircRx);
  (void)xTaskNotifyAndQueryFromISR(xTask, ulXfer, eSetValueWithOverwrite, &ulNotified, pxWoken);
  if (ulNotified & circ_rxDELETE_NOTIFIED) (void)xTaskNotifyFromISR(xTask, circ_rxDELETE_NOTIFIED, eSetBits, NULL);
}
