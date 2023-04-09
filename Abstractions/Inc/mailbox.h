/*
 * mailbox.h
 */

#pragma once

/*!
 * \file
 *
 * Echo mailbox task exemplar:
 * \code
 * static portTASK_FUNCTION(prvEchoMailboxTask, pvParameters) {
 *   vMailboxSetUp(pvParameters);
 *   MsgBindingHandle_t xMsgBinding = xMsgBindingNew();
 *   MsgUnifierHandle_t xMsgUnifier = xMsgUnifierNew();
 *   for (;;) {
 *     uint32_t ulNotified;
 *     xTaskNotifyWait(0UL, ULONG_MAX, &ulNotified, portMAX_DELAY);
 *     if (ulNotified & mailboxSENT_NOTIFIED)
 *       // Pattern-match the mailbox messages one by one. Do not wait for
 *       // additional messages, hence no ticks to wait.
 *       while (xMailboxReceiveMsg(NULL, xMsgUnifier, 0UL)) {
 *         if (xMsgUnify(xMsgUnifier) != eMsgUnifySuccess)
 *           continue;
 *
 *         // Match an optional mailbox sender handle but not by sending to itself.
 *         // Perform a basic sanity check. The sender must not be the receiver.
 *         MailboxHandle_t xMailbox;
 *         if (xMsgUnifyMailbox(xMsgUnifier, &xMailbox)) {
 *           if (xMailbox == xMailboxSelf())
 *             continue;
 *           if (xMsgUnify(xMsgUnifier) != eMsgUnifySuccess)
 *             continue;
 *         } else
 *           xMailbox = NULL;
 *
 *         // Handle ping by sending back pong.
 *         if (xMsgUnifyStrCmp(xMsgUnifier, "ping")) {
 *           if (xMailbox) {
 *             vMsgBindingClear(xMsgBinding);
 *             xMsgBindStr(xMsgBinding, "pong");
 *             xMailboxSendMsg(xMailbox, xMsgBinding, portMAX_DELAY);
 *             xMailboxSent(xMailbox);
 *             continue;
 *           }
 *         }
 *       }
 *   }
 *   vMsgUnifierDestroy(xMsgUnifier);
 *   vMsgBindingDestroy(xMsgBinding);
 *   vMailboxTearDown();
 *   vTaskDelete(NULL);
 * }
 * \endcode
 */

#include "FreeRTOS.h"

/*
 * The mailbox buffer size amounts to a small number of bytes by default.
 * 256 bytes suffices when using packed messaging.
 */
#define mailboxBUFFER_SIZE_BYTES 256U

#define mailboxSTACK_DEPTH 1024U

#define mailboxPRIORITY 24U

#if configUSE_TASK_NOTIFICATIONS == 1
#ifndef mailboxSENT_NOTIFIED
#define mailboxSENT_NOTIFIED (1UL << ('M' - 'A'))
#endif
#endif

#if configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0
#ifndef mailboxTASK_TLS_INDEX
#define mailboxTASK_TLS_INDEX 0U
#endif
#endif

/*!
 * \brief Handle of mailbox.
 *
 * The mailbox sends and receives messages via a message handle. Senders can
 * pass messages to tasks if a sender knows the mailbox.
 */
typedef struct Mailbox *MailboxHandle_t;

/*!
 * Asserts against memory allocation failures.
 */
MailboxHandle_t xMailboxCreate(size_t xBufferSizeBytes);

MailboxHandle_t xMailboxNew();

void vMailboxDelete(MailboxHandle_t xMailbox);

/*!
 * \brief Spawns the mailbox task.
 *
 * Notice that the mailbox is prime. Create the mailbox, spawn the mailbox. The
 * task appears to service the mailbox. The mailbox remains the message target
 * not the task.
 */
BaseType_t xMailboxSpawnWith(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode, const char *pcName,
                             const configSTACK_DEPTH_TYPE usStackDepth, UBaseType_t uxPriority);

BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode, const char *pcName);

/*!
 * \brief Links or unlinks two mailboxes.
 * \param[in] xLinking Handle of linking mailbox or \c NULL to unlink \c xLinked.
 * \param[in] xLinked Newly linked mailbox.
 *
 * Establishes link from one mailbox to another, a one-to-many association
 * between mailboxes.
 */
BaseType_t xMailboxLink(MailboxHandle_t xLinking, MailboxHandle_t xLinked);

void vMailboxUnlink(MailboxHandle_t xMailbox);

/*!
 * \brief Unlinks all mailboxes linked by the given mailbox.
 *
 * Makes a safe assumption about mutating a list while iterating the same list.
 */
void vMailboxUnlinkAll(MailboxHandle_t xMailbox);

BaseType_t xMailboxSetLinkOwner(MailboxHandle_t xMailbox, void *pvOwner);

void *pvMailboxGetLinkOwner(MailboxHandle_t xMailbox);

/*!
 * \brief Sets the link value of the mailbox.
 *
 * The link value determines the ordering of the mailbox within its link
 * association, and therefore the iteration order when yielding links. Defaults
 * to \c portMAX_DELAY which configures the mailbox for natural ordering where
 * the mailboxes appear in their link association in the order that they insert.
 */
BaseType_t xMailboxSetLinkValue(MailboxHandle_t xMailbox, TickType_t xValue);

TickType_t xMailboxGetLinkValue(MailboxHandle_t xMailbox);

MailboxHandle_t xMailboxLinkYield(MailboxHandle_t xMailbox, BaseType_t (*pxYield)(MailboxHandle_t xMailbox));

MailboxHandle_t xMailboxLinked(MailboxHandle_t xMailbox);

/*!
 * \brief Sends a message.
 *
 * Always pass messages by value, not by reference unless with great
 * caution.
 *
 * The message includes the sending mailbox.
 */
size_t xMailboxSend(MailboxHandle_t xMailbox, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait);

/*!
 * \brief Notifies mailbox message sent.
 * \returns \c pdPASS if the mailbox has spawned, i.e. its associated task
 * exists.
 *
 * Notify "sent" after sending one or more mailbox messages in order to prompt
 * the mailbox task. Mailbox receivers may periodically receive without specific
 * notification. The sent notification adds extra responsiveness and also makes
 * it easier for receivers to mesh with interrupt-level notifications.
 *
 * Use as follows.
 * \code
 * // Send a packed message. Wait indefinitely for the message to enter the
 * // mailbox's message buffer. Notify the receiver once the message has been
 * // recorded.
 * xMailboxSendMsg(xMailbox, xMsgPack, portMAX_DELAY);
 * xMailboxSent(xMailbox);
 * \endcode
 */
BaseType_t xMailboxSent(MailboxHandle_t xMailbox);

size_t vMailboxReceive(MailboxHandle_t xMailbox, void *pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait);

void vMailboxSetUp(void *pvParameters);

void vMailboxTearDown(void);

MailboxHandle_t xMailboxSelf();
