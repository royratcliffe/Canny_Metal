/*
 * tx_handler.h
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
 * \brief Abstract send handler.
 * \param pvSender Abstract sender context.
 * \param pvTxData Immutable data to send.
 * \param xDataLengthBytes Length of immutable data to send in bytes.
 *
 * Sends or discards. Does _not_ return a value, i.e. the number of
 * successfully-transmitted bytes, by design. The caller expects the handler to
 * block until the sender transmits all data bytes. The caller task only wants
 * to transfer the data. The handler always runs within the context of a task.
 *
 * The transmit handler must not retain the data, including by reference.
 * The handler should parse and retain only copies of the data as necessary.
 */
typedef void (*TxHandler_t)(void *pvSender, const void *pvTxData, size_t xDataLengthBytes);

void vTxMessageBufferHandler(void *pvMessageBuffer, const void *pvTxData, size_t xDataLengthBytes);

void vTxStreamBufferHandler(void *pvStreamBuffer, const void *pvTxData, size_t xDataLengthBytes);
