// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

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
