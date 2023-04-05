/*
 * mailbox.c
 */

/*
 * \file
 */

#include "mailbox_prv.h"

#include "task.h"

#include <memory.h>

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

#define mailboxSTACK_DEPTH 1024U

#define mailboxPRIORITY 24U

BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                         const char *const pcName) {
  return xMailboxSpawnWith(xMailbox, xTaskCode, pcName, mailboxSTACK_DEPTH,
                           mailboxPRIORITY);
}

/*!
 * \brief Sends a message.
 *
 * Always pass messages by value, never by reference unless with great
 * caution.
 *
 * The message includes the sending mailbox.
 */
size_t xMailboxSend(MailboxHandle_t xMailbox, const void *pvTxData,
                    size_t xDataLengthBytes, TickType_t xTicksToWait) {
  return xMessageBufferSend(xMailbox, pvTxData, xDataLengthBytes, xTicksToWait);
}
#if 0
BaseType_t
vMailboxSend (TaskHandle_t xTask, uint32_t ulValue, TickType_t xTime)
{
  BaseType_t xNotify = xTaskNotify(xTask, ulValue, eSetValueWithoutOverwrite);
  if (xNotify == pdFAIL)
    {
      SemaphoreHandle_t xSemaphore = pvTaskGetThreadLocalStoragePointer (
	  xTask, mailboxTHREAD_LOCAL_STORAGE_INDEX);
      if (xSemaphore)
	{
	  if (xSemaphoreTake(xSemaphore, xTime) == pdPASS)
	    {
	      xNotify = xTaskNotify(xTask, ulValue, eSetValueWithoutOverwrite);
	      xSemaphoreGive(xSemaphore);
	    }
	}
      else
	{
	  /*
	   * And what happens if the semaphore does not yet exist?
	   * This could happen if the sender sends *before* the receiver has both started
	   * and become ready to receive.
	   */
	  ;
	}
    }
  return xNotify;
}
#endif

void vMailboxReceive() { ; }
