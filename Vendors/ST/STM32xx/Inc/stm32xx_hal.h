#include "stm32xx.h"

/*
 * for HAL driver
 */
#if defined(USE_HAL_DRIVER)
#  if STM32xx == F4
#    include "stm32f4xx_hal.h"
#  endif /* STM32xx == F4 */
#endif   /* USE_HAL_DRIVER */
