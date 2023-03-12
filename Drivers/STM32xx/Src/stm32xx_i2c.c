/*
 * stm32xx_i2c.c STM32xx Drivers
 */

#include "stm32xx_i2c.h"

uint8_t ucCardinalForI2C(I2C_t xI2C)
{
	/*
	 * 5400, 0101 01xx --> 0
	 * 5800, 0101 10xx --> 1
	 * 5C00, 0101 11xx --> 2
	 * 8400, 1000 01xx --> 3
	 *
	 * In ARM assembler, this becomes:
	 *
	 *	lsrs	r0, r0, #10
	 *	tst		r0, #223
	 *	ite		ne
	 *	movne	r0, #30
	 *	moveq	r0, #21
	 *	bx		lr
	 */
	uint8_t xx = (uint32_t)xI2C >> 10U;
	return xx - (xx & 0x20U) ? 0x21U - 3U : 0x15U;
}

uint8_t ucOrdinalForI2C(I2C_t xI2C)
{
	/*
	 * Alternative implementation without branching:
	 *
	 *	uint8_t xx = (uint32_t)xI2C >> 10U;
	 *	return (xx & 0x03U) << ((xx & 0x20U) >> 4U);
	 */
	return 1U + ucCardinalForI2C(xI2C);
}

#ifdef HAL_I2C_MODULE_ENABLED

uint8_t ucI2CCardinal(I2CHandle_t xI2C)
{
	return ucCardinalForI2C(xI2C->Instance);
}

uint8_t ucI2COrdinal(I2CHandle_t xI2C)
{
	return ucOrdinalForI2C(xI2C->Instance);
}

#endif // ifdef HAL_I2C_MODULE_ENABLED
