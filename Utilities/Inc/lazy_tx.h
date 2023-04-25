// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

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
 * if the transmitter task priority sits low in the scheduler, say around
 * priority-level 16 by default assuming 24 approximates normal.
 *
 * By design, the lazy transmitter runs the handler whenever the time limit
 * expires even if the intrinsic stream buffer has no data to send. If no data,
 * it invokes the handler with a \c NULL transmit data pointer and a buffer
 * length of zero. Make \c lazytxDELAY_TICKS some number of useful ticks, e.g.
 * \c pdMS_TO_TICKS(1000U) for every second, for a periodic transmitter
 * heartbeat which a transmitter relay can use to reset a decoder.
 */
#define lazytxDELAY_TICKS portMAX_DELAY
#endif

#define lazytxSTACK_DEPTH (configMINIMAL_STACK_SIZE + lazytxBUFFER_LENGTH_BYTES)

#define lazytxPRIORITY 16U

typedef struct LazyTx *LazyTxHandle_t;

/*!
 * \brief Creates a lazy transmitter.
 * \param pvSender Abstract transmitter, a handle used by the handler.
 * \param xHandler Function to handle transmission, blocking if necessary.
 *
 * Dynamically creates the transmission stream. Use \c xLazyTxSend() to fill the
 * stream.
 */
LazyTxHandle_t xLazyTxCreate(void *pvSender, TxHandler_t xHandler);

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
