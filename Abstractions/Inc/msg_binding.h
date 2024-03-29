// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

/*!
 * \file
 *
 * Do not forget to predefine \c MSGPACK_SBUFFER_INIT_SIZE for the binding's packer.
 */

#include "FreeRTOS.h"
#include "message_buffer.h"

/*!
 * \defgroup MsgBinding Message Binding
 * \{
 */

typedef struct MsgBinding *MsgBindingHandle_t;

/*!
 * \brief Creates an new initialised message binding.
 *
 * Allocates the new binding using the FreeRTOS heap.
 */
MsgBindingHandle_t xMsgBindingNew();

void vMsgBindingDestroy(MsgBindingHandle_t xMsgBinding);

/*!
 * \brief Clears the message binding.
 */
void vMsgBindingClear(MsgBindingHandle_t xMsgBinding);

/*!
 * \brief Accesses the packed binding buffer.
 * \returns Size of buffer.
 */
size_t xMsgBindingBuffer(const MsgBindingHandle_t xMsgBinding, const void **ppvBuffer);

/*!
 * \brief Transfers binding buffer to message buffer.
 *
 * Supports multi-writer access to the message buffer by placing the send within
 * a critical section if \c xTicksToWait equals \c 0.
 */
size_t xMsgBindingSend(const MsgBindingHandle_t xMsgBinding, MessageBufferHandle_t xMessageBuffer,
                       TickType_t xTicksToWait);

/*!
 * \brief Binds a nil.
 *
 * Nil only has one value.
 */
BaseType_t xMsgBindNil(MsgBindingHandle_t xMsgBinding);

BaseType_t xMsgBindBool(MsgBindingHandle_t xMsgBinding, BaseType_t xValue);

BaseType_t xMsgBindInt(MsgBindingHandle_t xMsgBinding, int lValue);

BaseType_t xMsgBindUInt(MsgBindingHandle_t xMsgBinding, unsigned int ulValue);

BaseType_t xMsgBindFloat(MsgBindingHandle_t xMsgBinding, float lValue);

/*!
 * \brief Binds a string with its body.
 * \param[in] pcStr String pointer to character, the start of the string.
 * \param[in] xStrLengthBytes Length of string in bytes, not in Unicodes.
 *
 * The body should have UTF-8 encoding.
 */
BaseType_t xMsgBindStrWithBody(MsgBindingHandle_t xMsgBinding, const char *pcStr, size_t xStrLengthBytes);

BaseType_t xMsgBindStr(MsgBindingHandle_t xMsgBinding, const char *pcStr);

static inline BaseType_t xMsgBindStrNil(MsgBindingHandle_t xMsgBinding, const char *pcStr) {
  return xMsgBindStr(xMsgBinding, pcStr) && xMsgBindNil(xMsgBinding);
}

static inline BaseType_t xMsgBindStrBool(MsgBindingHandle_t xMsgBinding, const char *pcStr, BaseType_t xValue) {
  return xMsgBindStr(xMsgBinding, pcStr) && xMsgBindBool(xMsgBinding, xValue);
}

static inline BaseType_t xMsgBindStrInt(MsgBindingHandle_t xMsgBinding, const char *pcStr, int lValue) {
  return xMsgBindStr(xMsgBinding, pcStr) && xMsgBindInt(xMsgBinding, lValue);
}

static inline BaseType_t xMsgBindStrUInt(MsgBindingHandle_t xMsgBinding, const char *pcStr, unsigned int ulValue) {
  return xMsgBindStr(xMsgBinding, pcStr) && xMsgBindUInt(xMsgBinding, ulValue);
}

static inline BaseType_t xMsgBindStrFloat(MsgBindingHandle_t xMsgBinding, const char *pcStr, float lValue) {
  return xMsgBindStr(xMsgBinding, pcStr) && xMsgBindFloat(xMsgBinding, lValue);
}

BaseType_t xMsgBindArray(MsgBindingHandle_t xMsgBinding, size_t xNumberOfElements);

BaseType_t xMsgBindMap(MsgBindingHandle_t xMsgBinding, size_t xNumberOfEntries);

BaseType_t xMsgBindBinWithBody(MsgBindingHandle_t xMsgBinding, const void *pvBin, size_t xBinLengthBytes);

BaseType_t xMsgBindExtWithBody(MsgBindingHandle_t xMsgBinding, int8_t cType, const void *pvExt, size_t xExtLengthBytes);

/*!
 * \}
 */
