/*
 * mailbox_prv.h
 */

#pragma once

#include "mailbox.h"

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "task.h"

struct Mailbox {
  MessageBufferHandle_t xMessageBuffer;
  TaskHandle_t xTask;
  List_t xLinking;
  ListItem_t xLinked;
  volatile uint32_t ulSendingFailures;
};
