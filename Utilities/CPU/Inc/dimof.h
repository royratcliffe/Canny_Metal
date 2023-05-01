// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

/*!
 * \file macros.h
 * \brief Useful generic C macros.
 */

/*!
 * \brief Dimensions of array.
 * \param array Array expression.
 * \returns Dimension of array expression.
 *
 * Convenience macro for computing the ratio of an array's total size by the
 * size of its elements. Carefully encloses the array argument expression within
 * parentheses in order to avoid operator precedence problems when the parameter
 * passes a non-trivial expression.
 */
#define DIM_OF(array) (sizeof(array) / sizeof((array)[0]))
