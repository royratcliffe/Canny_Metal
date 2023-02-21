/*!
 * \file crc32.h
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

#pragma once

#include "FreeRTOS.h"

enum CRCRevIn
{
	CRCRevInNone,
	CRCRevInByte,
	CRCRevInHalfWord,
	CRCRevInWord
};

typedef enum CRCRevIn CRCRevIn_t;

enum CRCRevOut
{
	CRCRevOutDisabled,
	CRCRevOutEnabled
};

typedef enum CRCRevOut CRCRevOut_t;

BaseType_t xCRCGive();

BaseType_t xCRCTake(TickType_t xTicksToWait);

BaseType_t xCRCPoly32(uint32_t ulPol);

/*!
 * \brief Writes to the 32-bit initialisation register.
 */
void vCRCInit(uint32_t ulInit);

BaseType_t xCRCInputDataInversionNone();
BaseType_t xCRCInputDataInversionByte();
BaseType_t xCRCInputDataInversionHalfWord();
BaseType_t xCRCInputDataInversionWord();

BaseType_t xCRCRevIn(CRCRevIn_t xRevIn);

BaseType_t xCRCOutputDataInversionDisable();
BaseType_t xCRCOutputDataInversionEnable();

BaseType_t xCRCRevOut(CRCRevOut_t xRevOut);

/*!
 * The function exists as a convenience and also to recognise that typical usage
 * sets up bit reversing for input and output in tandom.
 */
BaseType_t xCRCRev(CRCRevIn_t xRevIn, CRCRevOut_t xRevOut);

/*!
 * \brief Configures the CRC unit for CRC-32 Castagnoli.
 *
 * Do not forget to invert the calculation result using the C `~` operator, or
 * equivalently by XOR with FFFFFFFF.
 *
 * \code
 *	{
 *		uint8_t ucData[] = {
 *				'1', '2', '3', '4', '5', '6', '7', '8', '9'
 *		};
 *		xCRC32C();
 *		uint32_t ulCRC32 = ~ulCRCCalc(ucData, sizeof(ucData));
 *	}
 * \endcode
 *
 * Note that the input buffer carefully excludes any terminators. C would
 * compile a null terminator if the byte array has a string initialiser.
 */
BaseType_t xCRC32C();

uint32_t ulCRCCalc(const void *pvData, size_t xDataLengthBytes);
