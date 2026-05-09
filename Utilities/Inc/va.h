/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file va.h
 * \brief Header file for variadic argument counting macros.
 * \details This file contains macros for counting the number of variadic
 * arguments passed to a macro. It can count up to 63 arguments and returns the
 * count as a compile-time constant. If there are no arguments, it returns 0. If
 * there are more than 63 arguments, it returns 63.
 */
#ifndef VA_H
#define VA_H

/*!
 * \brief Count number of variadic arguments.
 * \details This macro can count up to 63 arguments. If the number of arguments
 * is more than 63, it will return 63.
 * \param ... Variadic arguments.
 * \return Number of variadic arguments.
 * \note If there are no arguments, returns 0. Achieved by using ##__VA_ARGS__
 * which removes the comma if there are no arguments.
 * \note The use of ##__VA_ARGS__ is not supported by all compilers, such as
 * CCRX. For compilers that do not support ##__VA_ARGS__, the macro is defined
 * without it, which may lead to incorrect counting when no arguments are
 * provided.
 * \note The maximum number of arguments that can be counted is 63 due to the
 * use of reverse sequential numbers from 62 to 0 as padding. If more than 63
 * arguments are provided, the count will be capped at 63.
 * \note Why the initial argument is _0: The pasting operator \c ## concatenates
 * the initial argument _0 with the variadic arguments. This ensures that when
 * there are no variadic arguments, the macro still expands correctly and
 * returns 0. If there were no initial argument, the macro would not expand
 * correctly when no arguments are provided, leading to a compilation error.
 */
#if !defined(__CCRX__)
#define VA_NARGS(...) _VA_NARGS(_0, ##__VA_ARGS__, _VA_RSEQ63())
#else
#define VA_NARGS(...) _VA_NARGS(_0, __VA_ARGS__, _VA_RSEQ63())
#endif

/*!
 * \brief Expands variadic arguments and selects the 64th.
 * \param ... Variadic arguments padded with reverse sequential numbers.
 * \return Number of variadic arguments.
 */
#define _VA_NARGS(...) _VA_ARG64(__VA_ARGS__)

/*!
 * \brief Answers the 64th argument.
 * \details The 64th argument is determined by the position of the arguments.
 * Achieved by using reverse sequential numbers from 62 to 0 as padding. The
 * 64th position will correspond to the number of arguments.
 * \param ... Variadic arguments padded with reverse sequential numbers.
 * \return The 64th argument from the variadic arguments.
 */
#define _VA_ARG64(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
                  _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58,  \
                  _59, _60, _61, _62, _63, _64, ...)                                                                                                           \
  _64

/*!
 * \brief Reverse sequential numbers from 62 to 0.
 * \details Used to add padding to the variadic arguments so that the 64th
 * position corresponds to the number of arguments.
 */
#define _VA_RSEQ63()                                                                                                                                          \
  62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
      23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#endif /* VA_H */
