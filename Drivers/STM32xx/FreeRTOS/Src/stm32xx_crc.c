/*!
 * \file stm32xx_crc.c
 *
 * The CRC family of STM32 functions listed below present a very thin wrapping
 * around the HAL functions. They attempt to hide the HAL internal type
 * definitions and manifest constants as part of the private implementation. The
 * interface itself does not expose the HAL header. HAL error codes become pass
 * and fail, and all the FreeRTOS-oriented generic types and constants.
 */

#ifdef HAL_CRC_MODULE_ENABLED

#include "crc32.h"

#include "stm32xx_mcu.h"

/*
 * Activate the CRC; it appears under Computing within STM32CubeMX's Pinout and
 * Configuration panel. Include `crc.h`, typically under Core/Inc, in order to
 * access the external CRC handle `hcrc`.
 *
 *		extern CRC_HandleTypeDef hcrc;
 *
 * The Core auto-generated sources declare the CRC handle in header:
 *
 *		#include "crc.h"
 *
 * Compilation fails however unless the hardware abstraction layer enables its
 * CRC module.
 */
extern CRC_HandleTypeDef hcrc;

/*!
 * \brief Sets up the polynomial.
 * \param ulPol Generating polynomial. Contents of polynomial register, `POL`.
 * \param ulPolySize Polynomial size. One of 7, 8, 16 or 32 bits encoded as:
 * `CRC_POLYLENGTH_7B`, etc.
 * \retval Answers `pdPASS` on success.
 */
static BaseType_t prvCRCPoly(uint32_t ulPol, uint32_t ulPolySize)
{
	return HAL_CRCEx_Polynomial_Set(&hcrc, ulPol, ulPolySize) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCPoly32(uint32_t ulPol)
{
	return prvCRCPoly(ulPol, CRC_POLYLENGTH_32B);
}

void vCRCInit(uint32_t ulInit)
{
	WRITE_REG(hcrc.Instance->INIT, ulInit);
}

BaseType_t xCRCInputDataInversionNone()
{
	return HAL_CRCEx_Input_Data_Reverse(&hcrc, CRC_INPUTDATA_INVERSION_NONE) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCInputDataInversionByte()
{
	return HAL_CRCEx_Input_Data_Reverse(&hcrc, CRC_INPUTDATA_INVERSION_BYTE) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCInputDataInversionHalfWord()
{
	return HAL_CRCEx_Input_Data_Reverse(&hcrc, CRC_INPUTDATA_INVERSION_HALFWORD) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCInputDataInversionWord()
{
	return HAL_CRCEx_Input_Data_Reverse(&hcrc, CRC_INPUTDATA_INVERSION_WORD) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCRevIn(CRCRevIn_t xRevIn)
{
	static BaseType_t (*const xCRCInputDataInversion[])() =
	{
			xCRCInputDataInversionNone,
			xCRCInputDataInversionByte,
			xCRCInputDataInversionHalfWord,
			xCRCInputDataInversionWord
	};
	return (*xCRCInputDataInversion[xRevIn])();
}

BaseType_t xCRCOutputDataInversionDisable()
{
	return HAL_CRCEx_Output_Data_Reverse(&hcrc, CRC_OUTPUTDATA_INVERSION_DISABLE) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCOutputDataInversionEnable()
{
	return HAL_CRCEx_Output_Data_Reverse(&hcrc, CRC_OUTPUTDATA_INVERSION_ENABLE) == HAL_OK ? pdPASS : pdFAIL;
}

BaseType_t xCRCRevOut(CRCRevOut_t xRevOut)
{
	static BaseType_t (*const xCRCOutputDataInversion[])() =
	{
			xCRCOutputDataInversionDisable,
			xCRCOutputDataInversionEnable
	};
	return (*xCRCOutputDataInversion[xRevOut])();
}

BaseType_t xCRCRev(CRCRevIn_t xRevIn, CRCRevOut_t xRevOut)
{
	return xCRCRevIn(xRevIn) && xCRCRevOut(xRevOut);
}

BaseType_t xCRC32C()
{
	vCRCInit(0xffffffff);
	return xCRCPoly32(0x1EDC6F41) && xCRCRev(CRCRevInByte, CRCRevOutEnabled);
}

uint32_t ulCRCCalc(const void *pvData, size_t xDataLengthBytes)
{
	return HAL_CRC_Calculate(&hcrc, (void *)pvData, xDataLengthBytes);
}

#endif
