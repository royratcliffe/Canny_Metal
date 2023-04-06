/*
 * mailbox_msg.h
 */

#pragma once

#include "mailbox.h"
#include "msg.h"

/*!
 * \brief Message extension type for a mailbox handle.
 *
 * Extension type is a signed 8-bit integer of arbitrary but unique value.
 */
#ifndef msgEXT_TYPE_MAILBOX
#define msgEXT_TYPE_MAILBOX 'M'
#endif

size_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgPackHandle_t xMsgPack,
                       TickType_t xTicksToWait);
BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox,
                              MsgUnpackHandle_t xMsgUnpack,
                              TickType_t xTicksToWait);

BaseType_t xMsgPackMailbox(MsgPackHandle_t xMsgPack, MailboxHandle_t xMailbox);
BaseType_t xMsgUnpackMailbox(MsgUnpackHandle_t xMsgUnpack,
                             MailboxHandle_t *pxMailbox);

/*!
 * \brief Unpacks the next mailbox handle.
 * \param[in] xMsgUnpack Message unpacker handle.
 * \param[out] pxMailbox Pointer to unpacked mailbox handle.
 * \returns \c pdPASS if successfully unpacked a valid mailbox handle. Fails
 * with \c pdFAIL if the next unpack attempt does not succeed.
 *
 * Always unpacks the next object.
 */
BaseType_t xMsgUnpackNextMailbox(MsgUnpackHandle_t xMsgUnpack,
                                 MailboxHandle_t *pxMailbox);
