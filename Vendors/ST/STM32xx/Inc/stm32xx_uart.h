/*
 * stm32xx_uart.h
 */

#pragma once

#include "stm32xx_usart.h"

#define stm32xx_uartMAX_INSTANCES (stm32xx_usartMAX_INSTANCES)

#ifdef HAL_UART_MODULE_ENABLED

/*!
 * \brief UART handle
 *
 * Wraps the pointer to the UART structure as the handle type. Makes the pointer
 * become the handle, rather than a pointer to a handle. The HAL interface calls
 * the structure a handle for which it accepts a pointer reference.
 */
typedef UART_HandleTypeDef *UARTHandle_t;

/*!
 * \brief Function that handles a generic UART event.
 */
typedef void (*UARTHandler_t)(UARTHandle_t xUART);

/*!
 * \brief Function that handles a UART receiver event.
 */
typedef void (*UARTRxEventHandler_t)(UARTHandle_t xUART, uint16_t usXfer);

#endif
