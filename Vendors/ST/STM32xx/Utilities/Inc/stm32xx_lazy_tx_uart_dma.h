/*
 * stm32xx_lazy_tx_uart_dma.h
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

#include "lazy_tx.h"

#include "stm32xx_uart.h"

/*!
 * \brief Retry delay in ticks.
 *
 * Delay between failed attempts to start a DMA transmission.
 */
#define lazytx_uartdmaDELAY_TICKS 1UL

/*!
 * \brief Find lazy transmit handle for UART.
 *
 * \returns Lazy transmit handle for a given UART if configured for lazy
 * transmission, else \c NULL if not.
 */
LazyTxHandle_t xLazyTxForUART(UARTHandle_t hUART);

/*!
 * \brief Creates a lazy UART transmitter task via DMA on STM32.
 * \param hUART Pointer to STM32 UART hardware pre-configured for transmission
 * via DMA. The HAL interface calls this a handle albeit a non-opaque one.
 *
 * Registers a UART transmit complete callback. This requires per-UART
 * registration; the HAL disables registered callbacks by default.
 *
 * Requires UART interrupts enabled. Transmit complete callbacks only run if
 * interrupts enabled.
 */
LazyTxHandle_t xLazyTxUARTCreate(UARTHandle_t hUART);
