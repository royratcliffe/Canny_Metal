// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "stm32xx_uart.h"

#include "FreeRTOS.h"
#include "list.h"

ListItem_t *pxUARTRegisterTxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterTxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterRxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterRxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterError(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterAbortCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterAbortTransmitCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterAbortReceiveCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterWakeup(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

/*!
 * \brief Registers receive event handler for UART.
 * \param xUART Handle of UART with underlying USART instance.
 * \param xHandler Receive event handler.
 *
 * Receive events only fire on half- and full-buffer conditions by default when
 * using DMA transfers. Enable a UART's global interrupt to allow for a more
 * responsive performance where the receive event fires on an empty
 * receive-frame clock.
 *
 * Allows for more than one receive event handler per UART. Does not replace any
 * existing handler for the same UART.
 */
ListItem_t *pxUARTRegisterRxEvent(UARTHandle_t xUART, UARTRxEventHandler_t xHandler, TickType_t xDelay);

void vUARTUnregister(ListItem_t *pxRegisteredForUART);
