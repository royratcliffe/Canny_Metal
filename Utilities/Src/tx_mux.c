// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "tx_mux.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hand_side.h"

struct TxMux {
  struct hand_side *top;
};

TxMuxHandle_t xTxMuxCreate() {
  TxMuxHandle_t xTxMux = pvPortMalloc(sizeof(*xTxMux));
  configASSERT(xTxMux);
  return xTxMux;
}

void xTxMuxPush(TxMuxHandle_t xTxMux, void *pvSender, TxHandler_t xTxHandler) {
  (void)push_hand_side(&xTxMux->top, pvSender, xTxHandler);
}

static void *prvTxMuxSend(void *left, void *right, va_list args) {
  const void *pvTxData = va_arg(args, const void *);
  size_t xDataLengthBytes = va_arg(args, size_t);
  (*(TxHandler_t)right)(left, pvTxData, xDataLengthBytes);
  return NULL;
}

void vTxMuxSend(TxMuxHandle_t xTxMux, const void *pvTxData, size_t xDataLengthBytes) {
  vfor_left_and_right_hand_side(&xTxMux->top, prvTxMuxSend, pvTxData, xDataLengthBytes);
}
