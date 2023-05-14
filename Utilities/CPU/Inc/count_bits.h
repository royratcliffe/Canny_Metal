// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

/*!
 * \brief Counts number of bits in 32-bit integer using magic.
 *
 * Proof in R:
 * \code
 * ucCount32Bits <- Rcpp::cppFunction("uint8_t ucCount32Bits(uint32_t ulBits) {
 *   ulBits = ulBits - ((ulBits >> 1U) & 0x55555555U);
 *   ulBits = (ulBits & 0x33333333U) + ((ulBits >> 2U) & 0x33333333U);
 *   return (((ulBits + (ulBits >> 4U)) & 0x0f0f0f0fU) * 0x01010101U) >> 24U;
 * }")
 * \endcode
 * Gives you:
 * \code
 * > ucCount32Bits(0xffffffff) |> as.integer()
 * [1] 32
 * > ucCount32Bits(0xf0f0f0f0) |> as.integer()
 * [1] 16
 * > ucCount32Bits(0x00000000) |> as.integer()
 * [1] 0
 * \endcode
 * The pipe to \c as.integer() converts the \e raw result to a decimal integer.
 *
 * \sa http://graphics.stanford.edu/~seander/bithacks.html
 */
static inline uint8_t ucCount32Bits(uint32_t ulBits) {
  ulBits = ulBits - ((ulBits >> 1U) & 0x55555555U);
  ulBits = (ulBits & 0x33333333U) + ((ulBits >> 2U) & 0x33333333U);
  return (((ulBits + (ulBits >> 4U)) & 0x0f0f0f0fU) * 0x01010101U) >> 24U;
}
