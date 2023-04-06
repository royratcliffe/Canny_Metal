/*
 * mailbox_prv.h
 */

#pragma once

#include "mailbox.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

struct Mailbox {
  MessageBufferHandle_t xMessageBuffer;
  TaskHandle_t xTask;
};
