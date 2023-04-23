// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "rx_handler.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

size_t xRxMessageBufferHandler(void *pvMessageBuffer, void *pvRxBuffer, size_t xBufferLengthBytes) {
  return xMessageBufferReceive(pvMessageBuffer, pvRxBuffer, xBufferLengthBytes, portMAX_DELAY);
}

size_t xRxStreamBufferHandler(void *pvStreamBuffer, void *pvRxBuffer, size_t xBufferLengthBytes) {
  return xStreamBufferReceive(pvStreamBuffer, pvRxBuffer, xBufferLengthBytes, portMAX_DELAY);
}
