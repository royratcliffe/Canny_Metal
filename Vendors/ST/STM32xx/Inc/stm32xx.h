/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file stm32xx.h
 * \brief Common header for STM32xx devices.
 * \details This header file serves as a common include for all STM32xx devices,
 * providing a unified interface for including the appropriate device-specific
 * headers based on the target device being used. It defines macros to identify
 * the STM32 family and specific device, allowing for conditional compilation
 * and ensuring that the correct definitions and configurations are applied for
 * the target hardware. By including this header, developers can write code that
 * is portable across different STM32xx variants without needing to manually
 * change include statements or configurations for each specific device.
 */
#pragma once

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || \
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || \
    defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F469xx) || \
    defined(STM32F479xx) || defined(STM32F412Cx) || defined(STM32F412Rx) || defined(STM32F412Vx) || \
    defined(STM32F412Zx) || defined(STM32F413xx) || defined(STM32F423xx)
/*!
 * \brief Define STM32xx as the STM32 family being used.
 * \details This macro is defined based on the specific STM32F4xx device that is
 * being targeted. It allows the code to conditionally compile based on the
 * STM32 family, enabling support for multiple devices within the same codebase.
 * By defining STM32xx, the code can include the appropriate headers and use the
 * correct definitions for the specific device being used, ensuring
 * compatibility and proper functionality across different STM32F4xx variants.
 */
#  define STM32xx F4
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||  \
           STM32F429xx || STM32F439xx || STM32F401xC || STM32F401xE || STM32F410Tx || STM32F410Cx || \
           STM32F410Rx || STM32F411xE || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Cx || \
           STM32F412Rx || STM32F412Vx || STM32F412Zx || STM32F413xx || STM32F423xx */

#if defined(STM32F405xx)
#  define STM32F4xx F405
#elif defined(STM32F415xx)
#  define STM32F4xx F415
#elif defined(STM32F407xx)
#  define STM32F4xx F407
#elif defined(STM32F417xx)
#  define STM32F4xx F417
#elif defined(STM32F427xx)
#  define STM32F4xx F427
#elif defined(STM32F437xx)
#  define STM32F4xx F437
#elif defined(STM32F429xx)
#  define STM32F4xx F429
#elif defined(STM32F439xx)
#  define STM32F4xx F439
#elif defined(STM32F401xC)
#  define STM32F4xx F401xC
#elif defined(STM32F401xE)
#  define STM32F4xx F401xE
#elif defined(STM32F410Tx)
#  define STM32F4xx F410Tx
#elif defined(STM32F410Cx)
#  define STM32F4xx F410Cx
#elif defined(STM32F410Rx)
#  define STM32F4xx F410Rx
#elif defined(STM32F411xE)
#  define STM32F4xx F411xE
#elif defined(STM32F446xx)
#  define STM32F4xx F446xx
#elif defined(STM32F469xx)
#  define STM32F4xx F469xx
#elif defined(STM32F479xx)
#  define STM32F4xx F479xx
#elif defined(STM32F412Cx)
#  define STM32F4xx F412Cx
#elif defined(STM32F412Zx)
#  define STM32F4xx F412Zx
#elif defined(STM32F412Rx)
#  define STM32F4xx F412Rx
#elif defined(STM32F412Vx)
#  define STM32F4xx F412Vx
#elif defined(STM32F413xx)
#  define STM32F4xx F413xx
#elif defined(STM32F423xx)
#  define STM32F4xx F423xx
#endif

#if STM32xx == F4
#  include "stm32f4xx.h"
#endif /* STM32xx == F4 */
