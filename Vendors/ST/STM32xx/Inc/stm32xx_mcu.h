/*!
 * \file stm32xx_mcu.h
 *
 * Indirectly includes the HAL driver, assuming that the compiler has also
 * defined the `USE_HAL_DRIVER` manifest constant.
 *
 * The list of STM32 microprocessors and microcontrollers is large. The defines
 * below do not entirely encompass the currently existing range. ST continuously
 * expands its variants. Please add your own particular version.
 *
 * Should the "defined" block below run selectively with mutual exclusion using
 * `elif` pre-precessor directives? The design below makes them overlap in order
 * to trigger multiple-defintion errors if the compiler defines more than one
 * manifest constant simultaneously; choose one only.
 */

#pragma once

#if defined(STM32L452xx)
#define STM32xx L4
#define STM32L4xx 52
#endif

#if defined(STM32L4A6xx)
#define STM32xx L4
#define STM32L4xx A6
#endif

#if STM32xx == L4
#include "stm32l4xx.h"
#endif
