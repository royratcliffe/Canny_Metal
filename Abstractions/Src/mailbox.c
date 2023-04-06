/*
 * mailbox.c
 */

/*
 * \file
 */

#include "mailbox_prv.h"

#include <memory.h>
#include <stdio.h>

MailboxHandle_t xMailboxCreate(size_t xBufferSizeBytes) {
  MailboxHandle_t xMailbox = pvPortMalloc(sizeof(*xMailbox));
  configASSERT(xMailbox);
  (void)memset(xMailbox, 0, sizeof(*xMailbox));
  xMailbox->xMessageBuffer = xMessageBufferCreate(xBufferSizeBytes);
  return xMailbox;
}

MailboxHandle_t xMailboxNew() {
  return xMailboxCreate(mailboxBUFFER_SIZE_BYTES);
}

/*!
 * \brief Spawns the mailbox task.
 *
 * Notice that the mailbox is prime. Create the mailbox, spawn the mailbox. The
 * task appears to service the mailbox. The mailbox remains the message target
 * not the task.
 */
BaseType_t xMailboxSpawnWith(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                             const char *pcName,
                             const configSTACK_DEPTH_TYPE usStackDepth,
                             UBaseType_t uxPriority) {
  char name[configMAX_TASK_NAME_LEN];
  (void)snprintf(name, sizeof(name), "%s@%p", pcName ? pcName : "mailbox",
                 xMailbox);
  return xTaskCreate(xTaskCode, name, usStackDepth, xMailbox, uxPriority,
                     &xMailbox->xTask);
}

BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                         const char *const pcName) {
  return xMailboxSpawnWith(xMailbox, xTaskCode, pcName, mailboxSTACK_DEPTH,
                           mailboxPRIORITY);
}

/*!
 * \brief Sends a message.
 *
 * Always pass messages by value, not by reference unless with great
 * caution.
 *
 * The message includes the sending mailbox.
 */
size_t xMailboxSend(MailboxHandle_t xMailbox, const void *pvTxData,
                    size_t xDataLengthBytes, TickType_t xTicksToWait) {
  return xMessageBufferSend(xMailbox, pvTxData, xDataLengthBytes, xTicksToWait);
}

BaseType_t xMailboxSent(MailboxHandle_t xMailbox) {
#ifdef mailboxSENT_NOTIFIED
  if (xMailbox->xTask == NULL)
    return pdFAIL;
  return xTaskNotify(xMailbox->xTask, mailboxSENT_NOTIFIED, eSetBits);
#else
  return pdFAIL;
#endif
}

size_t vMailboxReceive(MailboxHandle_t xMailbox, void *pvRxData,
                       size_t xBufferLengthBytes, TickType_t xTicksToWait) {
  return xMessageBufferReceive(xMailbox, pvRxData, xBufferLengthBytes,
                               xTicksToWait);
}