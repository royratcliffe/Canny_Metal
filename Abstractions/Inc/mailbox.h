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

/*!
 * \brief Handle of mailbox.
 *
 * The mailbox sends and receives messages via a task handle. Senders can pass
 * messages if a sender knows the task. The messaging mechanism relies on task
 * notification, specifically on notification without overwrite.
 *
 * Define \c configNUM_THREAD_LOCAL_STORAGE_POINTERS in the firmware's FreeRTOS
 * configuration with at least one local pointer.
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

size_t vMailboxReceive(MailboxHandle_t xMailbox, void *pvRxData,
                       size_t xBufferLengthBytes, TickType_t xTicksToWait);
