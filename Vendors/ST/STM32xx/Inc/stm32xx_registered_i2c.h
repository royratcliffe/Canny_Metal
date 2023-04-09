/*
 * stm32xx_registered_i2c.h
 */

#pragma once

#include "stm32xx_i2c.h"

#ifdef HAL_I2C_MODULE_ENABLED

size_t xRegisteredCardinalOfI2C(I2CHandle_t xI2C);

#endif
