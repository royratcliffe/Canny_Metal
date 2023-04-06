/*
 * mailbox.h
 */

#pragma once

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

/*!
 * \brief Handle of mailbox.
 *
 * The mailbox sends and receives messages via a message handle. Senders can
 * pass messages to tasks if a sender knows the mailbox.
 */
typedef struct Mailbox *MailboxHandle_t;

MailboxHandle_t xMailboxNew();

BaseType_t xMailboxSpawnWith(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                             const char *pcName,
                             const configSTACK_DEPTH_TYPE usStackDepth,
                             UBaseType_t uxPriority);

BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                         const char *const pcName);

size_t xMailboxSend(MailboxHandle_t xMailbox, const void *pvTxData,
                    size_t xDataLengthBytes, TickType_t xTicksToWait);

/*!
 * \brief Notifies mailbox message sent.
 * \returns \c pdPASS if the mailbox has spawned, i.e. its associated task
 * exists.
 *
 * Notify "sent" after sending one or more mailbox messages in order to prompt
 * the mailbox task. Mailbox receivers may periodically receive without specific
 * notification. The sent notification adds extra responsiveness and also makes
 * it easier for receivers to mesh with interrupt-level notifications.
 */
BaseType_t xMailboxSent(MailboxHandle_t xMailbox);

size_t vMailboxReceive(MailboxHandle_t xMailbox, void *pvRxData,
                       size_t xBufferLengthBytes, TickType_t xTicksToWait);
