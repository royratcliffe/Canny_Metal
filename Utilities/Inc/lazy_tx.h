/*
 * lazy_tx.h
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

#include "tx_handler.h"

#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "task.h"

#define lazytxBUFFER_SIZE_BYTES 256UL

#define lazytxTRIGGER_LEVEL_BYTES 1UL

#define lazytxBUFFER_LENGTH_BYTES 32UL

#ifndef lazytxDELAY_TICKS
/*!
 * Having the delay too short makes the transmission task schedule too often, if
 * only to discover that the stream has nothing to transmit. This matters less
 * if the transmitter task priority sits low in the scheduler, say around 16 by
 * default assuming 24 approximates normal.
 */
#define lazytxDELAY_TICKS 1U
#endif

#define lazytxPRIORITY 16U

typedef struct LazyTx *LazyTxHandle_t;

/*!
 * \brief Creates a lazy transmitter.
 * \param pvSender Abstract transmitter, a handle used by handler.
 * \param Handler Function to handle transmission, blocking if necessary.
 *
 * Dynamically creates the transmission stream. Use xLazyTxSend() to fill the
 * stream.
 */
LazyTxHandle_t xLazyTxCreate(void *pvSender, TxHandler_t Handler);

/*!
 * \brief Answers the handle of the lazy transmitter task.
 *
 * Useful for task-level notifications.
 */
TaskHandle_t xLazyTxTaskHandle(LazyTxHandle_t xLazyTx);

/*!
 * \brief Sends data to a lazy transmitter.
 */
size_t xLazyTxSend(LazyTxHandle_t xLazyTx, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait);
