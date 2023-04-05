/*
 * mailbox_prv.h
 */

#pragma once

#include "mailbox.h"

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "semphr.h"

struct Mailbox {
  MessageBufferHandle_t xMessageBuffer;
  TaskHandle_t xTask;
  SemaphoreHandle_t xSemaphore;
};
