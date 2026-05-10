/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file cz.h
 * \brief Count leading and trailing zeros in 32-bit and 64-bit unsigned integers.
 * \details This header file provides inline functions for counting
 * leading zeros and trailing zeros in 32-bit and 64-bit unsigned
 * integers. It also includes functions for finding the least
 * significant bit (LSB) and most significant bit (MSB) in a 32-bit or
 * 64-bit unsigned integer. The functions use GCC's built-in intrinsics
 * for counting leading and trailing zeros, which are typically
 * optimised to use a single instruction on supported architectures. The
 * functions also handle the case where the input is zero, returning the
 * appropriate number of zeros in that case.
 */
#ifndef CZ_H
#define CZ_H

#include <stdint.h>

/*!
 * \brief Count leading zeros in a 32-bit unsigned integer.
 * \details This function returns the number of leading zeros in the
 * binary representation of the input integer `x`. If `x` is zero, it
 * returns 32, since all bits are zeros. This function uses the built-in
 * compiler intrinsic `__builtin_clz`, which is typically optimised to use
 * a single instruction on supported architectures.
 *
 * Leading zeros are the number of consecutive zero bits starting from
 * the most significant bit (MSB) down to the first one bit. For
 * example, if `x` is `0b00000000000000000000000000001010`, which is 10
 * in decimal, the function would return 28, since there are 28 leading
 * zeros before the first one bit.
 *
 * \param x The 32-bit unsigned integer to count leading zeros in.
 * \return The number of leading zeros in `x`, or 32 if `x` is zero.
 * \note The behaviour of GCC's `__builtin_clz` is undefined if `x` is
 * zero, which is why the function checks for that case explicitly and
 * returns 32.
 */
static inline int clz32(uint32_t x) { return x ? __builtin_clz(x) : 32; }

/*!
 * \brief Count leading zeros in a 64-bit unsigned integer.
 * \param x The 64-bit unsigned integer to count leading zeros in.
 * \return The number of leading zeros in `x`, or 64 if `x` is zero.
 * \note The behaviour of GCC's `__builtin_clzll` is undefined if `x` is
 * zero, which is why the function checks for that case explicitly and
 * returns 64.
 */
static inline int clz64(uint64_t x) { return x ? __builtin_clzll(x) : 64; }

/*!
 * \brief Count trailing zeros in a 32-bit unsigned integer.
 * \details Computes the number of trailing zeros in the binary
 * representation of the input integer `x`. If `x` is zero, it returns
 * 32, since all bits are zeros. This function uses the built-in
 * compiler intrinsic `__builtin_ctz`, which is typically optimised to
 * use a single instruction on supported architectures.
 *
 * Trailing zeros are the number of consecutive zero bits starting from
 * the least significant bit (LSB) up to the first one bit. For example,
 * if `x` is `0b00000000000000000000000000001010`, which is 10 in decimal,
 * the function would return 1, since there is 1 trailing zero before the
 * first one bit.
 *
 * \param x The 32-bit unsigned integer to count trailing zeros in.
 * \return The number of trailing zeros in `x`, or 32 if `x` is zero.
 * \note The behaviour of GCC's `__builtin_ctz` is undefined if `x` is
 * zero, which is why the function checks for that case explicitly and
 * returns 32.
 */
static inline int ctz32(uint32_t x) { return x ? __builtin_ctz(x) : 32; }

/*!
 * \brief Count trailing zeros in a 64-bit unsigned integer.
 * \param x The 64-bit unsigned integer to count trailing zeros in.
 * \return The number of trailing zeros in `x`, or 64 if `x` is zero.
 * \note The behaviour of GCC's `__builtin_ctzll` is undefined if `x` is
 * zero, which is why the function checks for that case explicitly and
 * returns 64.
 */
static inline int ctz64(uint64_t x) { return x ? __builtin_ctzll(x) : 64; }

/*!
 * \brief Find the least significant zero bit (LSZB) in a 32-bit unsigned integer.
 * \param x The 32-bit unsigned integer to find the LSZB in.
 * \return A 32-bit unsigned integer with only the least significant zero bit
 * of `x` set, or 0 if `x` is all ones.
 * \note The expression `~x & (x + 1)` is a common bit manipulation
 * technique to isolate the least significant zero bit in `x`. It works
 * because `x + 1` will flip all the bits up to and including the least
 * significant zero bit, and then `~x` will have ones in all the
 * positions where `x` has zeros. The bitwise AND of these two will
 * leave only the least significant zero bit set to 1.
 */
static inline uint32_t lszb32(uint32_t x) { return ~x & (x + 1); }

/*!
 * \brief Find the least significant zero bit (LSZB) in a 64-bit unsigned integer.
 * \param x The 64-bit unsigned integer to find the LSZB in.
 * \return A 64-bit unsigned integer with only the least significant zero bit
 * of `x` set, or 0 if `x` is all ones.
 */
static inline uint64_t lszb64(uint64_t x) { return ~x & (x + 1); }

/*!
 * \brief Find the most significant bit (MSB) in a 32-bit unsigned integer.
 * \param x The 32-bit unsigned integer to find the MSB in.
 * \return A 32-bit unsigned integer with only the MSB of `x` set, or 0
 * if `x` is zero.
 * \note This function uses the `clz32` function to determine the
 * position of the MSB. The expression `1U << (31 - clz32(x))`
 * calculates the value of the MSB by shifting a 1 to the left by the
 * appropriate number of positions based on the count of leading zeros.
 * \note It assumes that negative shift counts have \e undefined
 * behaviour in C. If `x` is zero, `clz32(x)` will return 32, and the
 * expression will effectively become `1U << (31 - 32)`, which is `1U <<
 * -1`, and since negative shifts are undefined, the function will
 * return 0 in that case, which is the correct result since there are no
 * bits set in zero.
 */
static inline uint32_t msb32(uint32_t x) { return x ? 1UL << (31 - clz32(x)) : 0UL; }

/*!
 * \brief Find the most significant bit (MSB) in a 64-bit unsigned integer.
 * \param x The 64-bit unsigned integer to find the MSB in.
 * \return A 64-bit unsigned integer with only the MSB of `x` set, or 0
 * if `x` is zero.
 */
static inline uint64_t msb64(uint64_t x) { return x ? 1ULL << (63 - clz64(x)) : 0ULL; }

#endif /* CZ_H */
