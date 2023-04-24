/*
 * \file stm32xx_uart_cb.h
 * \copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "stm32xx_uart.h"

#include "FreeRTOS.h"
#include "list.h"

ListItem_t *pxUARTRegisterTxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterTxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterRxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterRxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

ListItem_t *pxUARTRegisterError(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay);

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
