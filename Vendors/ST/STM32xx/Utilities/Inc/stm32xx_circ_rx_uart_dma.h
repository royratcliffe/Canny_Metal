/*
 * stm32xx_circ_rx_uart_dma.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
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
