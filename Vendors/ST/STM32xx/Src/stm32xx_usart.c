/*
 * stm32xx_usart.c
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

#include "stm32xx_usart.h"

uint8_t ucCardinalForUSART(USARTHandle_t xUSART) {
  /*
   * 4400, 0100 0100 --> 1
   * 4800, 0100 1000 --> 2
   * 4C00, 0100 1100 --> 3
   * 5000, 0101 0000 --> 4
   * 8000, 1000 0000 --> 0
   *
   * Proof in R computes as follows:
   *
   *      library(bitops)
   *      sapply(c(0x44, 0x48, 0x4c, 0x50, 0x80), \(xx) xx %>>% 2 %&% 7)
   *      # [1] 1 2 3 4 0
   */
  return (uint8_t)((uint32_t)xUSART >> 10U) & 7U;
}

uint8_t ucOrdinalForUSART(USARTHandle_t xUSART) { return ucCardinalForUSART(xUSART) + 1U; }
