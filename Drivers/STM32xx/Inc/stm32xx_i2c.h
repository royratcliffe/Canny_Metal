/*
 * stm32xx_i2c.h STM32xx Drivers
 */

#pragma once

#include "stm32xx_mcu.h"

#define stm32xx_i2cMAX_INSTANCES 4U

typedef I2C_TypeDef *I2C_t;

typedef I2C_HandleTypeDef *I2CHandle_t;

/*!
 * Note that the `xI2C` parameter refers to the I2C peripheral base address;
 * it does not refer to the HAL structure.
 */
uint8_t ucCardinalForI2C(I2C_t xI2C);

uint8_t ucOrdinalForI2C(I2C_t xI2C);
