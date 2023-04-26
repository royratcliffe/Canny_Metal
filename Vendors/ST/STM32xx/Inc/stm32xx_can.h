/*
 * stm32xx_can.h
 */

#pragma once

#include "stm32xx_mcu.h"

#define halcan_mailboxMAX_INSTANCES 3U

#define halcan_rxfifoMAX_INSTANCES 2U

typedef CAN_TypeDef *halCAN_t;

#ifdef HAL_CAN_MODULE_ENABLED

typedef CAN_HandleTypeDef *halCANHandle_t;

typedef void (*halCANHandler_t)(halCANHandle_t xhalCAN, ...);

#endif // ifdef HAL_CAN_MODULE_ENABLED
