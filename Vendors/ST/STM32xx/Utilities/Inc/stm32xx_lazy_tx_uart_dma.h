// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

/*!
 * \file
 *
 * Follow the example below to set up a lazy-circular transceiver that simply
 * sends back what it receives.
 *
 * \code
 * #include "FreeRTOS.h"
 * #include "task.h"
 *
 * #include "stm32xx_lazy_tx_uart_dma.h"
 * #include "stm32xx_circ_rx_uart_dma.h"
 *
 * // Core
 * #include "usart.h"
 *
 * #include <string.h>
 *
 * void prvLazyTxSend(void *pvSender, const void *pvTxData,
 *                    size_t xDataLengthBytes) {
 *   (void)xLazyTxSend(pvSender, pvTxData, xDataLengthBytes, portMAX_DELAY);
 * }
 *
 * portTASK_FUNCTION(StartDefaultTask, pvParameters) {
 *   LazyTxHandle_t xLazyTx = xLazyTxForUARTDMACreate(&huart2);
 *   char cData[] = "hello world\r\n";
 *   xLazyTxSend(xLazyTx, cData, strlen(cData), portMAX_DELAY);
 *   (void)xCircRxForUARTDMACreate(&huart2, xLazyTx, prvLazyTxSend);
 *   vTaskDelete(NULL);
 * }
 * \endcode
 *
 * Notice that the lazy transmitter and the circular receiver automatically
 * remain in memory. Once started, they both exist as high-priority tasks that
 * handle DMA interrupts, passing the incoming and outgoing data through stream
 * buffers.
 */

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
LazyTxHandle_t xLazyTxForUARTDMA(UARTHandle_t xUART);

/*!
 * \brief Creates a lazy UART transmitter task via DMA on STM32.
 *
 * \param xUART Pointer to STM32 UART hardware pre-configured for transmission
 * via DMA. The HAL interface calls this a handle albeit a non-opaque one.
 *
 * Registers a UART transmit complete callback. This requires per-UART
 * registration; the HAL disables registered callbacks by default.
 *
 * Requires UART interrupts enabled. Transmit complete callbacks only run if
 * interrupts enabled.
 */
LazyTxHandle_t xLazyTxForUARTDMACreate(UARTHandle_t xUART);
