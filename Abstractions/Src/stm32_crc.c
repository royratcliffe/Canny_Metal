// Abstractions stm32_crc.c
// SPDX-FileCopyrightText: Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

/*!
 * \file
 */

#include "stm32_crc.h"

#include "semphr.h"

static SemaphoreHandle_t xSemaphore = NULL;

void vCRCCreateMutex()
{
	xSemaphore = xSemaphoreCreateMutex();
	configASSERT(xSemaphore);
}

BaseType_t xCRCTake(TickType_t xTicksToWait)
{
	return xSemaphoreTake(xSemaphore, xTicksToWait);
}

BaseType_t xCRCGive()
{
	return xSemaphore ? xSemaphoreGive(xSemaphore) : pdFAIL;
}
