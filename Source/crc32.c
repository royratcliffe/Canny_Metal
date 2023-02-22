/*!
 * \file crc32.c
 *
 * Defines an abstract family of CRC peripheral operation functions. Imagine a
 * high-speed CRC computing peripheral that computes arbitrary CRC sums given
 * spans of bytes, half-words or words. The peripheral works by first setting up
 * the initial value, polynomial and bit reversing requirements. Subsequently,
 * use by sending a data pointer with a data length in bytes. The peripheral
 * returns a CRC result.
 *
 * The CRC family of STM32 functions listed below present a very thin wrapping
 * around the HAL functions. They attempt to hide the HAL internal type
 * definitions and manifest constants as part of the private implementation. The
 * interface itself does not expose the HAL header. HAL error codes become pass
 * and fail, and all the FreeRTOS-oriented generic types and constants.
 */

#include "crc32.h"

#include "semphr.h"

static SemaphoreHandle_t xSemaphore = NULL;

/*!
 * \brief Takes the CRC unit.
 *
 * Assumes a firmware environment where multiple tasks access the CRC unit. Uses
 * a mutex for priority inheritance, but cannot use from within interrupt
 * service routines. Lazily constructs the semaphore.
 */
BaseType_t xCRCTake(TickType_t xTicksToWait)
{
	if (xSemaphore == NULL)
	{
		xSemaphore = xSemaphoreCreateMutex();
		configASSERT(xSemaphore);
	}
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
