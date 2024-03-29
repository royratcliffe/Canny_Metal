// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "FreeRTOS.h"

#include "stm32xx_i2c.h"

#ifdef HAL_I2C_MODULE_ENABLED

BaseType_t xI2CGive(I2CHandle_t xI2C);

BaseType_t xI2CTake(I2CHandle_t xI2C, TickType_t xTicksToWait);

/*!
 * \brief Creates a recursive mutex for I2C sharing.
 *
 * Create the mutex first. The semaphore is static if FreeRTOS supports static
 * allocation; otherwise uses FreeRTOS heap.
 *
 * The recursive mutex allows nested locking operations for I2C.
 */
void vI2CCreateRecursiveMutex(I2CHandle_t xI2C);

#endif // ifdef HAL_I2C_MODULE_ENABLED
