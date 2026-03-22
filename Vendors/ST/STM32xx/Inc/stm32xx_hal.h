/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file stm32xx_hal.h
 * \brief HAL header for STM32xx series.
 * \details This header file includes the necessary HAL headers for the STM32xx
 * series. It conditionally includes the appropriate HAL header based on the
 * specific STM32xx series being used. The inclusion of the HAL driver is
 * controlled by the USE_HAL_DRIVER macro, allowing for flexibility in projects
 * that may not require the HAL driver. This header serves as a central point
 * for including the HAL functionality for STM32xx microcontrollers, simplifying
 * the inclusion process in other source files.
 */
#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

#include <stm32xx.h>

/*
 * for HAL driver
 */
#if defined(USE_HAL_DRIVER)
#  if STM32xx == F4
#    include "stm32f4xx_hal.h"
#  endif /* STM32xx == F4 */
#endif   /* USE_HAL_DRIVER */

#endif /* STM32xx_HAL_H */
