/*!
 * \file
 *
 * Indirectly includes the HAL driver, assuming that the compiler has also
 * defined the \c USE_HAL_DRIVER manifest constant.
 *
 * The list of STM32 microprocessors and micro-controllers is large. The defines
 * below do not entirely encompass the currently existing range. ST continuously
 * expands its variants. Please add your own particular version.
 *
 * Should the "defined" block below run selectively with mutual exclusion using
 * \c elif pre-precessor directives? The design below makes them overlap in
 * order to trigger multiple-defintion errors if the compiler defines more than
 * one manifest constant simultaneously; choose one only.
 */

#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#endif

#if defined(STM32L452xx)
#define STM32__ L4
#define STM32L4__ 52
#endif

#if defined(STM32L4A6xx)
#define STM32__ L4
#define STM32L4__ A6
#endif

#if STM32__ == L4
#include "stm32l4xx.h"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
