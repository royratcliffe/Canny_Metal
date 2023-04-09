/*
 * stm32xx_registered_uart.h
 */

#pragma once

#include "stm32xx_uart.h"

#ifdef HAL_UART_MODULE_ENABLED

#include <stddef.h>

/*!
 * \brief Cardinal for STM32 UART handle.
 *
 * Usage as follows.
 *
 * \code{.c}
 * // default.c
 *
 * #include "FreeRTOS.h"
 * #include "task.h"
 *
 * // Include the MX-generated Core header for USART peripherals.
 * #include "usart.h"
 *
 * #include "stm32xx_registered_uart.h"
 *
 * portTASK_FUNCTION(StartDefaultTask, pvParameters)
 * {
 *   // Register cardinal for LPUART1, if not already registered.
 *   size_t xCardinalOfLPUART1 = xRegisteredCardinalOfUART (&hlpuart1);
 *   vTaskDelete (NULL);
 * }
 * \endcode
 */
size_t xRegisteredCardinalOfUART(UART_HandleTypeDef *pxUART);

#endif
