/*
 * stm32xx_lazy_tx_uart_dma.c
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

#include "stm32xx_lazy_tx_uart_dma.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hand_side.h"

static struct hand_side *pLazyTxForUART = NULL;

LazyTxHandle_t xLazyTxForUART(UARTHandle_t hUART)
{
	return right_for_left_hand_side(&pLazyTxForUART, hUART);
}

/*!
 * Looks for success. Delays for a small amount of time on failure, including busy status.
 */
static void prvLazyTxHandler(void *pvTransmitter, const void *pvTxData, size_t xDataLengthBytes)
{
	while (HAL_UART_Transmit_DMA(pvTransmitter, pvTxData, xDataLengthBytes) != HAL_OK)
		vTaskDelay(lazytx_uartdmaDELAY_TICKS);
	(void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvTxCplt(UARTHandle_t hUART)
#else
void HAL_UART_TxCpltCallback(UARTHandle_t hUART)
#endif
{
	LazyTxHandle_t xLazyTx = xLazyTxForUART(hUART);
	if (xLazyTx) vTaskNotifyGiveFromISR(xLazyTxTaskHandle(xLazyTx), NULL);
}

LazyTxHandle_t xLazyTxUARTCreate(UARTHandle_t xUART)
{
	LazyTxHandle_t xLazyTx = xLazyTxCreate(xUART, prvLazyTxHandler);
	push_hand_side(&pLazyTxForUART, xUART, xLazyTx);
	/*
	 * Capturing the task handle is an alternative approach. This requires C11 standard. No straightforward
	 * alternative allows for a mapping between a UART handle and its task.
	 * Trouble is, this requires that the system has the ability to execute
	 * RAM-based thunks. Under normal circumstances, when firmware cannot execute RAM, attempts to bounce via
	 * thunks will trigger a fault.
	 *
	 *	void prvTxComplete(UART_HandleTypeDef *hUART)
	 *	{
	 *		vTaskNotifyGiveFromISR(xTask, NULL);
	 *	}
	 */
#if USE_HAL_UART_REGISTER_CALLBACKS
	HAL_UART_RegisterCallback(xUART, HAL_UART_TX_COMPLETE_CB_ID, prvTxCplt);
#endif
	return xLazyTx;
}
