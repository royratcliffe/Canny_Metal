// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "stm32xx_i2c.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "FreeRTOS.h"
#include "list.h"

typedef void (*I2CHandler_t)(I2CHandle_t xI2C);

ListItem_t *pxI2CRegisterMasterTxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxI2CRegisterMasterRxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxI2CRegisterSlaveTxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxI2CRegisterSlaveRxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

/*!
 * \param usAddrMatchCode 16-bit slave address.
 */
typedef void (*I2CAddrHandler_t)(I2CHandle_t xI2C, uint8_t ucTransferDirection, uint16_t usAddrMatchCode);

ListItem_t *pxI2CRegisterAddr(I2CHandle_t xI2C, I2CAddrHandler_t xHandler, TickType_t xDelay);

/*!
 * \brief Registers an I2C listen-complete handler.
 * \param[in] xDelay Nominal relative delay for handler used for the handler
 * list priority ordering. Pass \c portMAX_DELAY to add the handler to the end
 * of the handlers' list.
 */
ListItem_t *pxI2CRegisterListenCplt(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxI2CRegisterError(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxI2CRegisterAbort(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay);

/*!
 * \brief Unregisters registered handler items.
 * \param[in] pxRegisteredForI2C List item returned by registration.
 *
 * Registration allocates the registered list items dynamically. Unregistering
 * therefore requires removal of the list item then freeing of the item.
 */
void vI2CUnregister(ListItem_t *pxRegisteredForI2C);

#endif
