// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include <stdlib.h>

ptrdiff_t xCOBSStuff(const void *pvData, size_t xDataLengthBytes, void *pvCOBS);

/*!
 * Attempts to make the encoding process more efficient by fast scanning using
 * \c memchr and \c memcpy.
 *
 * \note Makes use of void pointer arithmetic.
 */
size_t xCOBSMemStuff(const void *pvData, size_t xDataLengthBytes, void *pvCOBS);

/*!
 * Un-stuffing checks the stuff spans for non-zero content.
 *
 * Spans of 255 have no zero terminator. Stuff code 0xff does \e not carry an
 * implied zero.
 */
ptrdiff_t xCOBSUnStuff(const void *pvCOBS, size_t xCOBSLengthBytes, void *pvData);

/*!
 * \param[in] pvCOBS Encoded bytes including its trailing null.
 *
 * \param[out] pvData Decoded bytes exluding the terminating null.
 *
 * \returns Number of decoded bytes. Answers \c 0 if invalid. Never answers zero
 * when valid. Valid data always terminates with a null and contains no other
 * nulls.
 *
 * Performs validation. All the incoming bytes must be nulls. In the case of an
 * invalid COBS sequence, the buffer may mutate for partial results if \c pvData
 * is not \c NULL.
 */
size_t xCOBSMemUnStuff(const void *pvCOBS, size_t xCOBSLengthBytes, void *pvData);
