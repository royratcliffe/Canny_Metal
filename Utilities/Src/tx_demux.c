/*
 * tx_demux.c
 */

#include "tx_demux.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hand_side.h"

struct TxDemux {
  struct hand_side *top;
};

typedef struct TxDemux TxDemux_t;

TxDemuxHandle_t xTxDemuxCreate() {
  TxDemux_t *pxTxDemux = pvPortMalloc(sizeof(TxDemux_t));
  configASSERT(pxTxDemux);
  return pxTxDemux;
}

void xTxDemuxPush(TxDemuxHandle_t xTxDemux, void *pvSender, TxHandler_t xTxHandler) {
  TxDemux_t *pxTxDemux = xTxDemux;
  (void)push_hand_side(&pxTxDemux->top, pvSender, xTxHandler);
}

static void *prvTxDemuxSend(void *left, void *right, va_list args) {
  const void *pvTxData = va_arg(args, const void *);
  size_t xDataLengthBytes = va_arg(args, size_t);
  (*(TxHandler_t)right)(left, pvTxData, xDataLengthBytes);
  return NULL;
}

void vTxDemuxSend(void *pvTxDemux, const void *pvTxData, size_t xDataLengthBytes) {
  TxDemux_t *pxTxDemux = pvTxDemux;
  vfor_left_and_right_hand_side(&pxTxDemux->top, prvTxDemuxSend, pvTxData, xDataLengthBytes);
}
