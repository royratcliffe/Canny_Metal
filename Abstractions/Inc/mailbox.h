/*
 * mailbox.h
 */

#pragma once

#include "FreeRTOS.h"

#ifndef mailboxTHREAD_LOCAL_STORAGE_INDEX
#define mailboxTHREAD_LOCAL_STORAGE_INDEX 0
#endif

/*
 * The mailbox buffer size amounts to a small number of bytes by default.
 * 256 bytes suffices when using packed messaging.
 */
#define mailboxBUFFER_SIZE_BYTES 256U

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
BaseType_t xMailboxSpawn(MailboxHandle_t xMailbox, TaskFunction_t xTaskCode,
                         const char *const pcName);
