/*
 * stm32xx_can_it.h
 */

#pragma once

#include "stm32xx_can.h"

#ifdef HAL_CAN_MODULE_ENABLED

#define CAN_IT_RX_FIFO0_ALL (CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_OVERRUN)
#define CAN_IT_RX_FIFO1_ALL (CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL | CAN_IT_RX_FIFO1_OVERRUN)
#define CAN_IT_ERROR_ALL \
  (CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR)

HAL_StatusTypeDef xCANActivateAllNotifications(CANHandle_t xCAN);

#endif // ifdef HAL_CAN_MODULE_ENABLED
