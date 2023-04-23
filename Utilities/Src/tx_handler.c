// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "tx_handler.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

void vTxMessageBufferHandler(void *pvMessageBuffer, const void *pvTxData, size_t xDataLengthBytes) {
  (void)xMessageBufferSend(pvMessageBuffer, pvTxData, xDataLengthBytes, portMAX_DELAY);
}

void vTxStreamBufferHandler(void *pvStreamBuffer, const void *pvTxData, size_t xDataLengthBytes) {
  (void)xStreamBufferSend(pvStreamBuffer, pvTxData, xDataLengthBytes, portMAX_DELAY);
}
