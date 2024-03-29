// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "stm32xx_i2c.h"

/*!
 * Enable the "Dual Address Acknowledged" parameter under Slave Features using
 * the STCubeMX tool. Also set up the Secondary Address Mask. Use "OA2[5:1]
 * masked" to match addresses 08 through 1F, unshifted. Similarly, 4:1 matches
 * 08 through 0F. OA stands for Own Address. Set up the "Secondary slave
 * address." Matching 7-bit addresses intersect the secondary address and the
 * mask, where the mask applies from the right---the second least significant
 * bit, shifted.
 */

#include "stm32xx_i2c_seq.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "FreeRTOS.h"

#ifndef i2cslaveBUFFER_LENGTH_BYTES
#define i2cslaveBUFFER_LENGTH_BYTES 256U
#endif

/*!
 * \brief Stack depth of I2C slave task in words.
 *
 * The depth needs to account for usage by device handlers. They respond to I2C
 * events within the context of the slave task.
 */
#ifndef i2cslaveSTACK_DEPTH
#define i2cslaveSTACK_DEPTH 2048U
#endif

#ifndef i2cslavePRIORITY
#define i2cslavePRIORITY 16U
#endif

#ifndef i2cslaveTASK_NAME
#define i2cslaveTASK_NAME "I2Cslave"
#endif

#ifndef i2cslaveMAX_DEVICES
#define i2cslaveMAX_DEVICES 16U
#endif

typedef struct I2CSlave *I2CSlaveHandle_t;

/*!
 * \brief Creates a new I2C slave.
 *
 * The slave operates in interrupt-driven mode by default. Creating does not
 * automatically start the slave; see \c xI2CSlaveStart.
 *
 * Creates and asserts a new slave. Halts if out of heap space.
 */
I2CSlaveHandle_t xI2CSlaveCreate(I2CHandle_t xI2C);

/*!
 * \brief Starts the I2C slave task.
 * \param[in] xI2CSlave Slave to start.
 * \param[in] pcName Name of slave task or \c NULL for default name.
 *
 * \note Assumes that the service task does not already exist.
 */
BaseType_t xI2CSlaveStart(I2CSlaveHandle_t xI2CSlave, const char *pcName);

/*!
 * \brief Stops the slave.
 */
void vI2CSlaveStop(I2CSlaveHandle_t xI2CSlave);

void vI2CSlaveDelete(I2CSlaveHandle_t xI2CSlave);

/*!
 * \defgroup stm32xxI2CSlaveDevice I2C Slave Device for STM32xx
 * \{
 */

void vI2CSlaveDeviceSlaveTxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveTxCplt)(I2CSeqHandle_t xI2CSeq));

void vI2CSlaveDeviceSlaveRxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveRxCplt)(I2CSeqHandle_t xI2CSeq));

/*!
 * \brief Sets up a slave device address handler.
 */
void vI2CSlaveDeviceAddr(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr, void (*pvAddr)(I2CSeqHandle_t xI2CSeq));

void vI2CSlaveDeviceError(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr, void (*pvError)(I2CSeqHandle_t xI2CSeq));

/*!
 * \}
 */

#endif // ifdef HAL_I2C_MODULE_ENABLED
