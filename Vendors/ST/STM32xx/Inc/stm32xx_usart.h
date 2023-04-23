/*
 * stm32xx_usart.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
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

#include "stm32xx_mcu.h"

#define stm32xx_usartMAX_INSTANCES 5U

typedef USART_TypeDef *USARTHandle_t;

/*!
 * \brief USART instance to eight-bit cardinal, very quickly.
 * \param xUSART Pointer to peripheral base address.
 * \returns Zero-based cardinal number for USART; 0 for LPUART1 in the low-power
 * peripheral range, 1 for USART2, 2 for USART3, etc. Always answers less than
 * unsigned value of \c stm32xx_usartMAX_INSTANCES manifest constant.
 *
 * Runs without looking up by sequential comparison. Instead relies on address
 * arithmetic for best performance and memory usage.
 *
 * The argument type ensures that the address matches a valid peripheral, not
 * just some random address. The implementation relies on the input being a
 * valid UART instance. This presumes that the HAL interface correctly maps the
 * peripheral space according to STM32L4xx expectations.
 */
uint8_t ucCardinalForUSART(USARTHandle_t xUSART);

/*!
 * \brief USART address to eight-bit ordinal.
 * \retval One-based ordinal number for USART.
 *
 * Answers between \c 1U and \c stm32xx_usartMAX_INSTANCES inclusive.
 */
uint8_t ucOrdinalForUSART(USARTHandle_t xUSART);
