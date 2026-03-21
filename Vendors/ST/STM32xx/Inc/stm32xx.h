/*!
 * \file stm32xx.h
 */
#pragma once

#if !defined(STM32F405xx) && !defined(STM32F415xx) && !defined(STM32F407xx) && !defined(STM32F417xx) && \
    !defined(STM32F427xx) && !defined(STM32F437xx) && !defined(STM32F429xx) && !defined(STM32F439xx) && \
    !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F410Tx) && !defined(STM32F410Cx) && \
    !defined(STM32F410Rx) && !defined(STM32F411xE) && !defined(STM32F446xx) && !defined(STM32F469xx) && \
    !defined(STM32F479xx) && !defined(STM32F412Cx) && !defined(STM32F412Rx) && !defined(STM32F412Vx) && \
    !defined(STM32F412Zx) && !defined(STM32F413xx) && !defined(STM32F423xx)
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
