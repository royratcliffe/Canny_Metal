/*
 * hand_side.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sublicense, and/or sell  copies  of   the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
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

#include <stdarg.h>

struct hand_side {
  void *left, *right;
  struct hand_side *pop;
};

/*!
 * \brief Pushes a new left and right
 * \param top Pointer to top
 * \param left Pointer to lefthand side
 * \param right Pointer to righthand side
 * \retval New left-right pair pushed to top or `NULL` if out of memory
 *
 * The push operation both pushes and creates if necessary. The top address is a
 * null pointer initially and this qualifies as a valid but empty hand-side
 * chain. Pushing the first left-right pair adds to the chain using
 * dynamically-allocated heap memory.
 *
 * Pushing fails if memory allocation fails. In such cases, the return value is
 * `NULL` and the hand-side chain remains unaltered. Success returns the new
 * top.
 */
struct hand_side *push_hand_side(struct hand_side **top, void *left, void *right);

/*!
 * \brief Pops left if found
 * \param top Top of chain
 * \param left Lefthand side to match
 * \retval Right hand side if match else `NULL`
 *
 * Removes left and right, matching on left. Removes only one, stopping at the
 * first match.
 */
void *pop_left_hand_side(struct hand_side **top, void *left);

/*!
 * \brief Frees left-right chain
 * \param top Pointer to top of chain
 *
 * Releases all the pairs and returns the chain to an empty condition.
 */
void free_hand_side(struct hand_side **top);

/*!
 * \brief Walks left-right chain
 * \param top Pointer to top of chain
 * \param and Pointer to function that visits left and right until it returns a
 * non-null result
 * \param arg Extra function argument
 * \retval Result of calling the function if non-null, else end of chain
 *
 * This generalised iteration function walks the left-right pairs one by one
 * calling a function with three arguments: the left, the right and some
 * context-related argument. The latter proves useful for older or deeply
 * embedded compilers that cannot or should not capture its context using nested
 * functions.
 *
 * The iteration stops when the function answers a non-null result. The
 * iteration itself returns null when no iterated function call gives a non-null
 * result.
 */
void *for_left_and_right_hand_side(struct hand_side **top, void *(*and)(void *left, void *right, void *arg), void *arg);

/*!
 * \brief Iterates left and right with variadic arguments.
 *
 * Restarts the variadic argument list at every iteration. Hence the `and`
 * function sees the same variable argument list albeit with differing left and
 * right pointers.
 */
void *vfor_left_and_right_hand_side(struct hand_side **top, void *(*and)(void *left, void *right, va_list args), ...);

/*!
 * \brief Right for lefthand side
 * \param top Pointer to top of chain
 * \param left Pointer to lefthand side
 *
 * Walks the chain looking for a matching left-hand side. Matching refers to
 * comparison by reference only. It compares the pointers. Answers the
 * right-hand side for the first match.
 *
 * Note that the search always stops at the first match, though technically
 * there could be more than one match if the chain receives more than one push
 * with the same left-hand side.
 */
void *right_for_left_hand_side(struct hand_side **top, void *left);

/*!
 * \brief Right for lefthand side by case-insensitive comparison
 * \param top Pointer to top of chain
 * \param left Pointer to null-terminated lefthand side string
 *
 * Right for left also supports insensitive string comparison where matching
 * utilises `stricmp` for comparing the left-hand side with the given string.
 * This naturally assumes that the left-hand sides represent pointers to
 * null-terminated strings. The implementation throws away the `const`
 * correctness by casting to the very generalised non-constant void pointer
 * type.
 */
void *right_for_left_stricmp_hand_side(struct hand_side **top, const char *left);

/*!
 * \brief Left for righthand side
 * \param top Pointer to top of chain
 * \param right Pointer to righthand side
 *
 * Lookups typically work from left to right, but not always. When pairing
 * strings (that is, pointers to null-terminated constant characters) with
 * arbitrary pointers, for example, a reverse translation could prove handly by
 * answering the string for some given pointer.
 */
void *left_for_right_hand_side(struct hand_side **top, void *right);
