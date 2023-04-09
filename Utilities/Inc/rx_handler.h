/*
 * rx_handler.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sublicense, and/or sell  copies  of   the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
