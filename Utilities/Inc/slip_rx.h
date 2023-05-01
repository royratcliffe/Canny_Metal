/*
 * slip_rx.h
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

#include "slip.h"

#include "tx_handler.h"

#ifndef sliprxMAX_PACKET_LEN
#ifndef slipMAX_PACKET_LEN
#define sliprxMAX_PACKET_LEN 256U
#else
#define sliprxMAX_PACKET_LEN (slipMAX_PACKET_LEN)
#endif
#endif

#ifndef sliprxSTACK_DEPTH
#define sliprxSTACK_DEPTH (configMINIMAL_STACK_SIZE + sliprxMAX_PACKET_LEN)
#endif

#ifndef sliprxPRIORITY
#define sliprxPRIORITY 16U
#endif

typedef struct SLIPRx *SLIPRxHandle_t;

/*!
 * \brief Sends a received packet.
 * \param pvSender Packet transmit relay context.
 */
typedef TxHandler_t SLIPRxSendHandler_t;

/*!
 * \brief Creates a new SLIP receiver dynamically.
 *
 * Creates a new receiver task with the necessary stack size, amounting to
 * minimal size plus the maximum packet length. Reserves sufficient heap space
 * for the new task and its stack.
 */
SLIPRxHandle_t xSLIPRxCreate(size_t xBufferSizeBytes, size_t xTriggerLevelBytes);

void vSLIPRxSpawn(SLIPRxHandle_t xSLIPRx, const char *pcName);

void vSLIPRxSendHandler(SLIPRxHandle_t xSLIPRx, void *pvSender, SLIPRxSendHandler_t xHandler);

/*!
 * \brief Sends a packet.
 *
 * Filters out zero-length packets. Sending no bytes amounts to a no-operation;
 * it always returns immediately.
 */
void vSLIPRxSend(SLIPRxHandle_t xSLIPRx, const void *pvData, size_t xDataLengthBytes);

/*!
 * Deletes SLIP receiver infrastructure.
 *
 * Deletes task and stream buffer in reverse order.
 */
void vSLIPRxDelete(SLIPRxHandle_t xSLIPRx);
