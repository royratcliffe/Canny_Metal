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
 * if the transmitter task priority sits low in the scheduler, say around 16 by
 * default assuming 24 approximates normal.
 */
#define lazytxDELAY_TICKS portMAX_DELAY
#endif

#define lazytxSTACK_DEPTH (configMINIMAL_STACK_SIZE + lazytxBUFFER_LENGTH_BYTES)

#define lazytxPRIORITY 16U

typedef struct LazyTx *LazyTxHandle_t;

/*!
 * \brief Creates a lazy transmitter.
 * \param pvSender Abstract transmitter, a handle used by handler.
 * \param xHandler Function to handle transmission, blocking if necessary.
 *
 * Dynamically creates the transmission stream. Use xLazyTxSend() to fill the
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
