// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "tx_handler.h"

typedef struct TxMux *TxMuxHandle_t;

TxMuxHandle_t xTxMuxCreate();

void xTxMuxPush(TxMuxHandle_t xTxMux, void *pvSender, TxHandler_t xTxHandler);

/*!
 * \brief Sends data using the transmit handler signature.
 */
void vTxMuxSend(TxMuxHandle_t xTxMux, const void *pvTxData, size_t xDataLengthBytes);
