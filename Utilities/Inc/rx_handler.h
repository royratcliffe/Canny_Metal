// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include <stddef.h>

/*!
 * \brief Abstract receive handler.
 *
 * Receives zero or more bytes blocking if necessary. The interface does _not_
 * poll its receiver. Instead, it typically blocks the caller task until one or
 * more bytes appear.
 *
 * The interface lends itself to reuse for either (1) progressively received
 * octet streams, or (2) for receiving frames of octets as single clipped
 * datagram units. In the first case, the return value counts the currently
 * buffered span of octets, where as the size returned describes the number of
 * frame octets.
 */
typedef size_t (*RxHandler_t)(void *pvReceiver, void *pvRxBuffer, size_t xBufferLengthBytes);

/*!
 * \brief Receives one complete message from a message buffer.
 *
 * The message could be empty, have zero length.
 */
size_t xRxMessageBufferHandler(void *pvMessageBuffer, void *pvRxBuffer, size_t xBufferLengthBytes);

/*!
 * \brief Receives zero or more data bytes from a stream buffer.
 */
size_t xRxStreamBufferHandler(void *pvStreamBuffer, void *pvRxBuffer, size_t xBufferLengthBytes);
