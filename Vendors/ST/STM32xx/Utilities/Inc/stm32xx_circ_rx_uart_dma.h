// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "circ_rx.h"

#include "stm32xx_uart.h"

#ifndef circrx_uartdmaBUFFER_LENGTH_BYTES
#define circrx_uartdmaBUFFER_LENGTH_BYTES 32U
#endif

#define circrx_uartdmaDELAY_TICKS 1U

#ifndef circrx_uartdmaSTACK_DEPTH
#define circrx_uartdmaSTACK_DEPTH (configMINIMAL_STACK_SIZE + circrx_uartdmaBUFFER_LENGTH_BYTES)
#endif

#define circrx_uartdmaPRIORITY 16U

CircRxHandle_t xCircRxForUARTDMA(UARTHandle_t xUART);

/*!
 * \brief Follows one-to-one association from circular receiver to UART handle.
 *
 * Performance is poorer than UART-to-receiver mapping. Searches all available
 * cardinals until match or failure.
 */
UARTHandle_t xUARTDMAForCircRx(CircRxHandle_t xCircRx);

/*!
 * \brief Creates a new circular receiver.
 * \param[in] pvSender User data for target of outgoing data bytes.
 * \param[in] xHandler Transmit relay handler.
 * \returns Newly-allocated circular receiver wired up for UART reception via DMA.
 *
 * Use as follows where \c xHandler identifies a relay transmission function, i.e.
 * somewhere to pass bytes on successful receipt. The \c NULL passes to the
 * function's first parameter, and ignored in this excerpt.
 * \code{.c}
 * xCircRxUARTDMACreate(&hlpuart1, NULL, handler);
 * \endcode
 * Uses dynamic memory for space allocation. Expression \c &lpuart1 identifies
 * the low-power UART channel to wire up for receiving using DMA. The channel
 * needs a circular DMA channel.
 */
CircRxHandle_t xCircRxUARTDMACreate(UARTHandle_t xUART, void *pvSender, TxHandler_t xHandler);

/*!
 * \brief Deletes and unwinds a circular DMA-based UART receiver.
 */
void vCircRxUARTDMADelete(UARTHandle_t xUART);
