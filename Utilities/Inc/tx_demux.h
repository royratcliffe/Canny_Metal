/*
 * tx_demux.h
 */

#pragma once

#include "tx_handler.h"

typedef void *TxDemuxHandle_t;

TxDemuxHandle_t xTxDemuxCreate();

void xTxDemuxPush(TxDemuxHandle_t xTxDemux, void *pvSender, TxHandler_t xTxHandler);

/*!
 * \brief Sends data using the transmit handler signature.
 */
void vTxDemuxSend(void *pvTxDemux, const void *pvTxData, size_t xDataLengthBytes);
