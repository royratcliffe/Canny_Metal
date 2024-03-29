/*
 * stm32xx_i2c.h
 */

#pragma once

#include "stm32xx_mcu.h"

#define stm32xx_i2cMAX_INSTANCES 4U

typedef I2C_TypeDef *I2C_t;

/*!
 * \brief Cardinal integer for underlying I2C instance.
 *
 * \note Note that the \c xI2C parameter refers to the I2C peripheral base address;
 * it does not refer to the HAL structure.
 */
uint8_t ucCardinalForI2C(I2C_t xI2C);

uint8_t ucOrdinalForI2C(I2C_t xI2C);

/*
 * The STM32 headers do not define \c I2C_HandleTypeDef unless the project
 * enables the I2C module. Hence make its dependencies likewise compile-time
 * elided by the same manifest constant.
 */
#ifdef HAL_I2C_MODULE_ENABLED

typedef I2C_HandleTypeDef *I2CHandle_t;

uint8_t ucI2CCardinal(I2CHandle_t xI2C);

uint8_t ucI2COrdinal(I2CHandle_t xI2C);

#endif
