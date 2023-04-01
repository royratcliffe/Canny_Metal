/*
 * stm32xx_i2c.h
 */

#pragma once

#include "stm32xx_mcu.h"

#define stm32xx_i2cMAX_INSTANCES 4U

/*
 * The STM32 headers do not define \c I2C_HandleTypeDef unless the project
 * enables the I2C module. Hence make its dependencies likewise compile-time
 * elided by the same manifest constant.
 */
#ifdef HAL_I2C_MODULE_ENABLED

typedef I2C_HandleTypeDef *I2CHandle_t;

#endif
