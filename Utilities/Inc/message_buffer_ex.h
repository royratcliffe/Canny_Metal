/*
 * message_buffer_ex.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
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

#include "FreeRTOS.h"
#include "message_buffer.h"

/*!
 * \brief Receives a message from a stream buffer.
 *
 * \param xMessageBuffer Handle of message buffer from which to receive.
 *
 * \param ppvRxData Pointer to dynamic buffer pointer. Must not be \c NULL. Free
 * the buffer after processing the message using \c pvPortFree().
 *
 * \param xTicksToWait How long to wait for the next message in ticks.
 *
 * \returns Number of bytes received, equal to the size of the message and the
 * number of bytes waiting in the heap-allocated dynamic buffer. Answers zero if
 * memory allocation fails.
 *
 * The implementation first attempts to read the message by blocking on a
 * single-byte read. Doing so waits for at least one pending message. The
 * one-byte receive will fail if the message exceeds a single byte in length.
 * Failure does not discard the pending message. Its length sits in the stream
 * buffer.
 */
size_t xMessageBufferReceiveAll(MessageBufferHandle_t xMessageBuffer, void **ppvRxData, TickType_t xTicksToWait);

/*!
 * Receives the entire next message dynamically.
 *
 * \param pvRxData Pointer to received data pointer typed as eight-bit unsigned
 * bytes. Typically points to an automatic stack-allocated buffer space else
 * some pre-allocated block of heap.
 *
 * \returns Number of bytes received. The answer \e could be zero if the message
 * buffer contains a zero-length message.
 *
 * The implementation relies on the all-or-nothing interface deployed by message
 * buffers: receive the entire message or read nothing. In this case, zero
 * received bytes indicates failure rather than a successful zero-length
 * message.
 *
 * Importantly assumes that the maximum size of the message does not exceed the
 * receive data buffer. The buffer will otherwise overrun, crashing its stack
 * boundary. This pre-condition requires that the message sender applies the
 * limiting threshold at the transmission point.
 */
size_t xMessageBufferReceiveMax(MessageBufferHandle_t xMessageBuffer, void *pvRxData, TickType_t xTicksToWait);
