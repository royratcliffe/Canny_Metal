/*!
 * \file crc32.c
 */

#include "crc32.h"

#include "semphr.h"

static SemaphoreHandle_t xSemaphore = NULL;

void vCRCCreateMutex()
{
	xSemaphore = xSemaphoreCreateMutex();
	configASSERT(xSemaphore);
}

/*!
 * \brief Takes the CRC unit.
 *
 * Assumes a firmware environment where multiple tasks access the CRC unit. Uses
 * a mutex for priority inheritance, but cannot use from within interrupt
 * service routines.
 */
BaseType_t xCRCTake(TickType_t xTicksToWait)
{
	return xSemaphoreTake(xSemaphore, xTicksToWait);
}

/*!
 * \brief Gives back the CRC unit.
 *
 * Fails and does nothing if the semaphore does not yet exist.
 */
BaseType_t xCRCGive()
{
	return xSemaphore ? xSemaphoreGive(xSemaphore) : pdFAIL;
}
