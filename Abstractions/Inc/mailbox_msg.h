/*
 * mailbox_msg.h
 */

#pragma once

#include "mailbox.h"

#include "msg_binding.h"
#include "msg_unifier.h"

/*!
 * \brief Message extension type for a mailbox handle.
 *
 * Extension type is a signed 8-bit integer of arbitrary but unique value.
 */
#ifndef msgEXT_TYPE_MAILBOX
#define msgEXT_TYPE_MAILBOX 'M'
#endif

/*!
 * \brief Sends a message binding to a mailbox.
 *
 * \param[in] xMailbox Handle of mailbox receiver. Can be \c NULL and defaults
 * to self. Mailboxes can send messages to themselves but take care not to
 * generate infinite loops.
 *
 * You can send the \e same message binding to multiple mailboxes. Sending does
 * \e not change the binding.
 *
 * Send a message to a mailbox as follows.
 * \code
 * // Ping the echo mailbox. Bind a task handle followed by a string.
 * // Send the message then notify.
 * MsgBindingHandle_t xMsgBinding = xMsgBindingNew();
 * xMsgBindMailbox(xMsgBinding, NULL);
 * xMsgBindStr(xMsgBinding, "ping");
 * xMailboxSendMsg(xMailbox, xMsgBinding, portMAX_DELAY);
 * vMsgBindingDestroy(xMsgBinding);
 * xMailboxSent(xMailbox);
 * \endcode
 */
BaseType_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding, TickType_t xTicksToWait);

/*!
 * \brief Sends message to mailbox without every blocking.
 *
 * Silently fails if the mailbox parameter resolves to no mailbox; it does \e
 * not send a message. Otherwise, it never waits for the mailbox's message
 * buffer to find space for the message send to complete successfully. Instead,
 * it silently fails.
 *
 * Use this version of sending for multiple-sender scenarios.
 *
 * Automatically notifies "sent" if the destingation mailbox task already exists
 * and the bytes were successfully sent. This might lead to multiple
 * notifications when sending more than one message. The task will absorb the
 * multiplicity by folding them to a single wake up. However, depending on
 * relative priorities, the sends could conceivably overlap the receiving
 * mailbox handler and leave a trailing redundant no-message notification.
 */
void vMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding);

/*!
 * \brief Receives a message from a mailbox.
 * \param[in] xMailbox Mailbox to receive from, else \c NULL for the current
 * task's mailbox.
 * \param[out] xMsgUnifier Unifier for mailbox message.
 * \returns \c pdPASS if one message received else \c pdFAIL.
 */
BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait);

/*!
 * \brief Relays a previously-received message.
 * \param[in] xMsgUnifier Carries the received message. May or may not have
 * already been unified, either partially or completely.
 */
BaseType_t xMailboxRelayMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait);

BaseType_t xMsgBindMailbox(MsgBindingHandle_t xMsgBinding, MailboxHandle_t xMailbox);

BaseType_t xMsgUnifyMailbox(MsgUnifierHandle_t xMsgUnifier, MailboxHandle_t *pxMailbox);

/*!
 * \brief Unifies the next mailbox handle.
 * \param[in] xMsgUnifier Message unifier handle.
 * \param[out] pxMailbox Pointer to unpacked mailbox handle.
 * \returns \c pdPASS if successfully unpacked a valid mailbox handle. Fails
 * with \c pdFAIL if the next unpack attempt does not succeed.
 *
 * Always unpacks the next object.
 */
BaseType_t xMsgUnifierNextMailbox(MsgUnifierHandle_t xMsgUnpack, MailboxHandle_t *pxMailbox);
