/*
 * stm32xx_i2c_callback.h
 *
 *  Created on: Mar 11, 2023
 *      Author: roy.ratcliffe
 */

#pragma once

#include "stm32xx_i2c.h"

void vI2CRegisterSlaveTxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler);
