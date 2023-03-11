/*
 * stm32xx_i2c_slave.h
 *
 *  Created on: Mar 11, 2023
 *      Author: RoyRatcliffe
 */

#pragma once

#include "stm32xx_mcu.h"

#include "FreeRTOS.h"

typedef I2C_HandleTypeDef *I2CHandle_t;

#ifndef i2cslaveBUFFER_LENGTH_BYTES
#define i2cslaveBUFFER_LENGTH_BYTES 256U
#endif

#ifndef i2cslaveSTACK_DEPTH
#define i2cslaveSTACK_DEPTH (configMINIMAL_STACK_SIZE + i2cslaveBUFFER_LENGTH_BYTES)
#endif

#ifndef i2cslavePRIORITY
#define i2cslavePRIORITY 16U
#endif

#ifndef i2cslaveNAME
#define i2cslaveNAME "i2cslave"
#endif

typedef void *I2CSlaveHandle_t;

/*!
 * The transmit data does *not* carry a `const` qualifier because it points to FreeRTOS-allocated memory which the slave task frees on transmit completion.
 */
typedef size_t (*I2CSlaveHandler_t)(const void *pvRxData, size_t xDataLengthBytes, void **ppvTxData);

/*!
 * \brief Creates a new I2C slave.
 *
 * The slave operates in interrupt-driven mode by default.
 */
I2CSlaveHandle_t xI2CSlaveCreate(I2CHandle_t xI2C);

/*!
 * \brief Starts the I2C slave task.
 * Assumes that the service task does not already exist.
 */
BaseType_t xI2CSlaveStart(I2CSlaveHandle_t xI2CSlave, const char *pcName);

/*!
 * \brief Stops the slave.
 */
void vI2CSlaveStop(I2CSlaveHandle_t xI2CSlave);

/*!
 * \brief Sets up slave receive and transmit.
 */
void vI2CSlaveUseDMA(I2CSlaveHandle_t xI2CSlave);

void vI2CSlaveAddressHandler(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddress, I2CSlaveHandler_t xHandler);
