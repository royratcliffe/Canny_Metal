/*
 * slip_tx.c
 */

#include "slip_tx.h"

#include "task.h"
#include "message_buffer_ex.h"

#include <string.h>

struct SLIPTx {
  MessageBufferHandle_t xMessageBuffer;
  void *pvSender;
  TxHandler_t xHandler;
  TaskHandle_t xTask;
};

static portTASK_FUNCTION(prvSLIPTxCode, pvParameters) {
  SLIPTxHandle_t xSLIPTx = pvParameters;
  for (;;) {
    /*
     * Using xMessageBufferReceiveMax makes an important assumption.
     * The message buffer's maximum size must never exceed sliptxMAX_PACKET_LEN.
     */
    uint8_t ucData[sliptxMAX_PACKET_LEN];
    size_t xReceivedBytes = xMessageBufferReceiveMax(xSLIPTx->xMessageBuffer, ucData, portMAX_DELAY);

    /*
     * Encode the frame. Runs the encoding one or two bytes at a time. This
     * results in multiple invocations of the send handler.
     *
     * The name of the data is "transmit data" even though the transmit task
     * _receives_ it. The term "transmit" here refers to the pipeline as a whole
     * at the architectural level.
     */
    static uint8_t ucEnd[] = {SLIP_END};
    static uint8_t ucEscEnd[] = {SLIP_ESC, SLIP_END};
    static uint8_t ucEscEsc[] = {SLIP_ESC, SLIP_ESC};
    vSLIPTxSend(xSLIPTx, ucEnd, sizeof(ucEnd));
    for (size_t ux = 0UL; ux < xReceivedBytes; ux++) {
      switch (ucData[ux]) {
      case SLIP_END:
        vSLIPTxSend(xSLIPTx, ucEscEnd, sizeof(ucEscEnd));
        break;
      case SLIP_ESC:
        vSLIPTxSend(xSLIPTx, ucEscEsc, sizeof(ucEscEsc));
        break;
      default:
        vSLIPTxSend(xSLIPTx, ucData + ux, sizeof(*ucData));
      }
    }
    vSLIPTxSend(xSLIPTx, ucEnd, sizeof(ucEnd));
  }
}

SLIPTxHandle_t xSLIPTxCreate(size_t xBufferSizeBytes) {
  SLIPTxHandle_t xSLIPTx = pvPortMalloc(sizeof(*xSLIPTx));
  configASSERT(xSLIPTx != NULL);
  (void)memset(xSLIPTx, 0, sizeof(*xSLIPTx));
  xSLIPTx->xMessageBuffer = xMessageBufferCreate(xBufferSizeBytes);
  configASSERT(xSLIPTx->xMessageBuffer);
  xTaskCreate(prvSLIPTxCode, "SLIPtx", sliptxSTACK_DEPTH, xSLIPTx, sliptxPRIORITY, &xSLIPTx->xTask);
  configASSERT(xSLIPTx->xTask != NULL);
  return xSLIPTx;
}

void vSLIPTxSendHandler(SLIPTxHandle_t xSLIPTx, void *pvSender, SLIPTxSendHandler_t xHandler) {
  xSLIPTx->pvSender = pvSender;
  xSLIPTx->xHandler = xHandler;
}

void vSLIPTxSend(SLIPTxHandle_t xSLIPTx, const void *pvTxData, size_t xDataLengthBytes) {
  if (xSLIPTx->xHandler) xSLIPTx->xHandler(xSLIPTx->pvSender, pvTxData, xDataLengthBytes);
}

void vSLIPTxDelete(SLIPTxHandle_t xSLIPTx) {
  vTaskDelete(xSLIPTx->xTask);
  vMessageBufferDelete(xSLIPTx->xMessageBuffer);
  vPortFree(xSLIPTx);
}
