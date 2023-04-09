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
 */
BaseType_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding, TickType_t xTicksToWait);

/*!
 * \brief Receives a message from a mailbox.
 * \param[in] xMailbox Mailbox to receive from, else \c NULL for the current
 * task's mailbox.
 * \param[out] xMsgUnifier Unifier for mailbox message.
 * \returns \c pdPASS if one message received else \c pdFAIL.
 */
BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait);

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
