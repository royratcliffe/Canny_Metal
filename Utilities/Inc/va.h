/*!
 * \brief Count number of variadic arguments.
 * \details This macro can count up to 63 arguments. If the number of arguments
 * is more than 63, it will return 63.
 * \param ... Variadic arguments.
 * \return Number of variadic arguments.
 * \note If there are no arguments, it will return 0. Achieved by using
 * ##__VA_ARGS__ which removes the comma if there are no arguments.
 */
#if !defined(__CCRX__)
#define VA_NARGS(...) _VA_NARGS(_0, ##__VA_ARGS__, _VA_RSEQ())
#else
#define VA_NARGS(...) _VA_NARGS(_0, __VA_ARGS__, _VA_RSEQ())
#endif

/*!
 * \brief Get the number of variadic arguments.
 * \details This macro is implemented by using the _VA_NTHARG macro to get the
 * Nth argument from the variadic arguments. Reverse sequential numbers from 62
 * to 0 are used as padding so that the Nth position corresponds to the number
 * of arguments.
 * \param ... Variadic arguments padded with reverse sequential numbers.
 * \return Number of variadic arguments.
 */
#define _VA_NARGS(...) _VA_NTHARG(__VA_ARGS__)

/*!
 * \brief Get the Nth argument from the variadic arguments.
 * \details This macro is used to get the Nth argument from the variadic
 * arguments. The Nth argument is determined by the position of the arguments.
 * Achieved by using reverse sequential numbers from 62 to 0 as padding.
 * The Nth position will correspond to the number of arguments.
 * \param ... Variadic arguments padded with reverse sequential numbers.
 * \return The Nth argument from the variadic arguments.
 */
#define _VA_NTHARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29,     \
                   _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, \
                   _58, _59, _60, _61, _62, _63, N, ...)                                                                                                       \
  N

/*!
 * \brief Reverse sequential numbers from 62 to 0.
 * \details This macro is used to add padding to the variadic arguments so
 * that the Nth position corresponds to the number of arguments. For example,
 * if there are 3 arguments, the 3rd position will be 3, and the 4th position
 * will be 2, and so on.
 */
#define _VA_RSEQ()                                                                                                                                            \
  62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
      23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
