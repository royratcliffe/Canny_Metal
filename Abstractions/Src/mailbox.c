/*
 * mailbox.c
 */

/*
 * \file
 */

#include "mailbox_prv.h"

#include <memory.h>
#include <stdio.h>

#include "containerof.h"

MailboxHandle_t xMailboxCreate(size_t xBufferSizeBytes) {
  MailboxHandle_t xMailbox = pvPortMalloc(sizeof(*xMailbox));
  configASSERT(xMailbox);
  (void)memset(xMailbox, 0, sizeof(*xMailbox));
  xMailbox->xMessageBuffer = xMessageBufferCreate(xBufferSizeBytes);
  configASSERT(xMailbox->xMessageBuffer);
  vListInitialise(&xMailbox->xLinking);
  vListInitialiseItem(&xMailbox->xLinked);
  listSET_LIST_ITEM_OWNER(&xMailbox->xLinked, NULL);
  listSET_LIST_ITEM_VALUE(&xMailbox->xLinked, portMAX_DELAY);
  return xMailbox;
}

MailboxHandle_t xMailboxNew() { return xMailboxCreate(mailboxBUFFER_SIZE_BYTES); }

void vMailboxDelete(MailboxHandle_t xMailbox) {
  if (xMailbox == NULL) return;
  vMailboxUnlink(xMailbox);
  vMailboxUnlinkAll(xMailbox);
  vMessageBufferDelete(xMailbox->xMessageBuffer);
  vPortFree(xMailbox);
}

BaseType_t xMailboxSpawnWith(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode, const char *pcName,
                             const configSTACK_DEPTH_TYPE usStackDepth, UBaseType_t uxPriority) {
  char name[configMAX_TASK_NAME_LEN];
  (void)snprintf(name, sizeof(name), "%s@%p", pcName ? pcName : "mailbox", xMailbox);
  return xTaskCreate(xTaskCode, name, usStackDepth, xMailbox, uxPriority, &xMailbox->xTask);
}

BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode, const char *pcName) {
  return xMailboxSpawnWith(xMailbox, xTaskCode, pcName, mailboxSTACK_DEPTH, mailboxPRIORITY);
}

BaseType_t xMailboxLink(MailboxHandle_t xLinking, MailboxHandle_t xLinked) {
  if (xLinking == xLinked) return pdFAIL;
  /*
   * Switch from one linking list to another, or from some list to none,
   * entirely atomically.
   */
  taskENTER_CRITICAL();
  {
    if (xLinked) {
      if (listLIST_ITEM_CONTAINER(&xLinked->xLinked) != NULL) (void)uxListRemove(&xLinked->xLinked);
      if (xLinking) vListInsert(&xLinking->xLinking, &xLinked->xLinked);
    }
  }
  taskEXIT_CRITICAL();
  return pdPASS;
}

void vMailboxUnlink(MailboxHandle_t xMailbox) {
  taskENTER_CRITICAL();
  {
    if (listLIST_ITEM_CONTAINER(&xMailbox->xLinked) != NULL) (void)uxListRemove(&xMailbox->xLinked);
  }
  taskEXIT_CRITICAL();
}

void vMailboxUnlinkAll(MailboxHandle_t xMailbox) {
  BaseType_t prvYield(MailboxHandle_t xLinked) {
    vMailboxUnlink(xLinked);
    return pdFAIL;
  }
  (void)xMailboxYieldLinked(xMailbox, prvYield);
}

BaseType_t xMailboxSetLinkOwner(MailboxHandle_t xMailbox, void *pvOwner) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  listSET_LIST_ITEM_OWNER(&xMailbox->xLinked, pvOwner);
  return pdPASS;
}

void *pvMailboxGetLinkOwner(MailboxHandle_t xMailbox) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return NULL;
  return listGET_LIST_ITEM_OWNER(&xMailbox->xLinked);
}

BaseType_t xMailboxSetLinkValue(MailboxHandle_t xMailbox, TickType_t xValue) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  listSET_LIST_ITEM_VALUE(&xMailbox->xLinked, xValue);
  return pdPASS;
}

TickType_t xMailboxGetLinkValue(MailboxHandle_t xMailbox) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return portMAX_DELAY;
  return listGET_LIST_ITEM_VALUE(&xMailbox->xLinked);
}

MailboxHandle_t xMailboxYieldLinked(MailboxHandle_t xMailbox, BaseType_t (*pxYield)(MailboxHandle_t xMailbox)) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return NULL;
  for (const ListItem_t *pxHeadEntry = listGET_HEAD_ENTRY(&xMailbox->xLinking),
                        *pxEndMarker = listGET_END_MARKER(&xMailbox->xLinking), *pxNextEntry;
       pxHeadEntry != pxEndMarker; pxHeadEntry = pxNextEntry) {
    MailboxHandle_t xLinked = CONTAINER_OF(pxHeadEntry, struct Mailbox, xLinked);
    /*
     * Allow for removal of the link during iteration. Step to the next link of
     * the mailbox before yielding.
     */
    pxNextEntry = listGET_NEXT(pxHeadEntry);
    if (pxYield(xLinked) == pdPASS) return xLinked;
  }
  return NULL;
}

MailboxHandle_t xMailboxLinking(MailboxHandle_t xMailbox) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return NULL;
  List_t *pxLinking = listLIST_ITEM_CONTAINER(&xMailbox->xLinked);
  return pxLinking != NULL ? CONTAINER_OF(pxLinking, struct Mailbox, xLinking) : NULL;
}

size_t xMailboxSend(MailboxHandle_t xMailbox, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return 0UL;
  return xMessageBufferSend(xMailbox, pvTxData, xDataLengthBytes, xTicksToWait);
}

BaseType_t xMailboxSent(MailboxHandle_t xMailbox) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
#ifdef mailboxSENT_NOTIFIED
  if (xMailbox->xTask == NULL) return pdFAIL;
  return xTaskNotify(xMailbox->xTask, mailboxSENT_NOTIFIED, eSetBits);
#else
  return pdFAIL;
#endif
}

size_t vMailboxReceive(MailboxHandle_t xMailbox, void *pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return 0UL;
  return xMessageBufferReceive(xMailbox, pvRxData, xBufferLengthBytes, xTicksToWait);
}

void vMailboxSetUp(void *pvParameters) {
#ifdef mailboxTASK_TLS_INDEX
  vTaskSetThreadLocalStoragePointer(NULL, mailboxTASK_TLS_INDEX, pvParameters);
#endif
}

void vMailboxTearDown() {
#ifdef mailboxTASK_TLS_INDEX
  vTaskSetThreadLocalStoragePointer(NULL, mailboxTASK_TLS_INDEX, NULL);
#endif
}

MailboxHandle_t xMailboxSelf() {
#ifdef mailboxTASK_TLS_INDEX
  return pvTaskGetThreadLocalStoragePointer(NULL, mailboxTASK_TLS_INDEX);
#else
  return NULL;
#endif
}
