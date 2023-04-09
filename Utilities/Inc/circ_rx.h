/*
 * circ_rx.h
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

#include <stddef.h>

#include "tx_handler.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

typedef void *CircRxHandle_t;

/*!
 * \brief Sends data.
 *
 * The circular receiver _sends_ its data onward to some other relay or sink but
 * remains agnostic about who or what that might be. Does nothing if no handler
 * assigned as yet.
 */
void vCircRxSend(CircRxHandle_t xCircRx, const void *pvTxData, size_t xDataLengthBytes);

/*!
 * \brief Continuously receives buffered data bytes by receiving transfer offset
 * notifications.
 *
 * The implementation synchronises with the task notification value continuously
 * and stores the running transfer offset and the circular transfer buffer on
 * the task stack.
 */
void vCircRx(CircRxHandle_t xCircRx, void *pvRxBuffer, size_t xBufferLengthBytes);

/*!
 * \brief Creates a new dynamically-allocated circular receiver.
 *
 * Does _not_ automatically create an associated task. Initially has no task.
 */
CircRxHandle_t xCircRxCreate(void *pvSender, TxHandler_t Handler);

/*!
 * \brief Wires up a stream-buffered circular receiver.
 *
 * The receiver blocks indefinitely when filling the given stream buffer. This
 * implies that transfer to the stream buffer will always succeed.
 */
void vCircRxStreamBuffer(CircRxHandle_t xCircRx, StreamBufferHandle_t xStreamBuffer);

/*!
 * \brief Task handle getter.
 */
TaskHandle_t xCircRxTaskHandle(CircRxHandle_t xCircRx);

/*!
 * \brief Task handle setter.
 */
void vCircRxTaskHandle(CircRxHandle_t xCircRx, TaskHandle_t xTask);

/*!
 * \brief Notifies circular receiver task.
 * \param ulXfer New circular transfer offset.
 * \param pxWoken Assigned to `pdTRUE` if context switch required because a
 * higher-priority task woke up.
 */
BaseType_t xCircRxNotifyFromISR(CircRxHandle_t xCircRx, uint32_t ulXfer, BaseType_t *pxWoken);
