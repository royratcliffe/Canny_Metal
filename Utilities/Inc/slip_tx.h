/*
 * slip_tx.h
 */

#pragma once

#include "slip.h"

#include "tx_handler.h"

#ifndef sliptxMAX_PACKET_LEN
#ifndef slipMAX_PACKET_LEN
#define sliptxMAX_PACKET_LEN 256U
#else
#define sliptxMAX_PACKET_LEN (slipMAX_PACKET_LEN)
#endif
#endif

#ifndef sliptxSTACK_DEPTH
#define sliptxSTACK_DEPTH (configMINIMAL_STACK_SIZE + sliptxMAX_PACKET_LEN)
#endif

#ifndef sliptxPRIORITY
#define sliptxPRIORITY 16U
#endif

/*
 * \brief Handle of SLIP transmitter.
 *
 * The transmitter encapsulates two things: a message buffer on which the
 * transmitter blocks and a transmitter function with its opaque parameter. The
 * SLIP transmitter eats incoming un-encoded raw frames and spits out a
 * SLIP-encoded byte stream.
 */
typedef struct SLIPTx *SLIPTxHandle_t;

typedef TxHandler_t SLIPTxSendHandler_t;

SLIPTxHandle_t xSLIPTxCreate(size_t xBufferSizeBytes);

void vSLIPTxSpawn(SLIPTxHandle_t xSLIPTx, const char *pcName);

void vSLIPTxSendHandler(SLIPTxHandle_t xSLIPTx, void *pvSender, SLIPTxSendHandler_t xHandler);

/*
 * \brief Sends raw data from the SLIP transmitter.
 *
 * Used privately but also exposed for diagnostic purposes, bypassing the SLIP
 * encoder.
 */
void vSLIPTxSend(SLIPTxHandle_t xSLIPTx, const void *pvData, size_t xDataLengthBytes);

void vSLIPTxDelete(SLIPTxHandle_t xSLIPTx);
