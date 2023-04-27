/*
 * stm32xx_can.h
 */

#pragma once

#include "stm32xx_mcu.h"

#define stm32xx_canTX_MAILBOX_MAX_INSTANCES 3U

#define stm32xx_canRX_FIFO_MAX_INSTANCES 2U

typedef CAN_TypeDef *CAN_t;

#ifdef HAL_CAN_MODULE_ENABLED

typedef CAN_HandleTypeDef *CANHandle_t;

typedef void (*CANTxMailboxHandler_t)(CANHandle_t xCAN, uint8_t ucTxMailbox);

typedef void (*CANRxFifoHandler_t)(CANHandle_t xCAN, uint8_t ucRxFifo);

typedef void (*CANHandler_t)(CANHandle_t xCAN);

#endif // ifdef HAL_CAN_MODULE_ENABLED
