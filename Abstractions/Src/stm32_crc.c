/*!
 * \file stm32_crc.c
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
