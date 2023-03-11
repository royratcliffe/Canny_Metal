/*
 * circ_rx.c
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
 *		The above copyright notice and this permission notice shall be
 *		included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "circ_rx.h"

#include "tx_handler.h"

#include <limits.h>

struct CircRx
{
	TaskHandle_t xTask;
	TxHandler_t xHandler;
	void *pvSender;
};

typedef struct CircRx CircRx_t;

void vCircRxSend(CircRxHandle_t xCircRx, const void *pvTxData, size_t xDataLengthBytes)
{
	if (xDataLengthBytes == 0UL) return;
	CircRx_t *pxCircRx = xCircRx;
	if (pxCircRx->xHandler == NULL) return;
	(*pxCircRx->xHandler)(pxCircRx->pvSender, pvTxData, xDataLengthBytes);
}

void vCircRx(CircRxHandle_t xCircRx, void *pvRxBuffer, size_t xBufferLengthBytes)
{
	uint32_t ul = 0UL;
	for (;;)
	{
		uint32_t ulXfer;
		xTaskNotifyWait(0UL, ULONG_MAX, &ulXfer, portMAX_DELAY);
		if (ul == ulXfer) continue;
		if (ulXfer > ul)
			vCircRxSend(xCircRx, (uint8_t *)pvRxBuffer + ul, ulXfer - ul);
		else
		{
			vCircRxSend(xCircRx, (uint8_t *)pvRxBuffer + ul, xBufferLengthBytes - ul);
			vCircRxSend(xCircRx, pvRxBuffer, ulXfer);
		}
		ul = ulXfer;
	}
}

CircRxHandle_t xCircRxCreate(void *pvSender, TxHandler_t xHandler)
{
	CircRx_t *pxCircRx = pvPortMalloc(sizeof(*pxCircRx));
	configASSERT(pxCircRx);
	pxCircRx->xTask = NULL;
	pxCircRx->xHandler = xHandler;
	pxCircRx->pvSender = pvSender;
	return pxCircRx;
}

TaskHandle_t xCircRxTaskHandle(CircRxHandle_t xCircRx)
{
	CircRx_t *pxCircRx = xCircRx;
	return pxCircRx->xTask;
}

void vCircRxTaskHandle(CircRxHandle_t xCircRx, TaskHandle_t xTask)
{
	CircRx_t *pxCircRx = xCircRx;
	pxCircRx->xTask = xTask;
}

void vCircRxStreamBuffer(CircRxHandle_t xCircRx, StreamBufferHandle_t xStreamBuffer)
{
	CircRx_t *pxCircRx = xCircRx;
	pxCircRx->xHandler = vTxStreamBufferHandler;
	pxCircRx->pvSender = xStreamBuffer;
}

BaseType_t xCircRxNotifyFromISR(CircRxHandle_t xCircRx, uint32_t ulXfer, BaseType_t *pxWoken)
{
	return xTaskNotifyFromISR(xCircRxTaskHandle(xCircRx), ulXfer, eSetValueWithOverwrite, pxWoken);
}
