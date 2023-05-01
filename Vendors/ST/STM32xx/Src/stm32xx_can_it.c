// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_can_it.h"

#ifdef HAL_CAN_MODULE_ENABLED

HAL_StatusTypeDef xCANActivateAllNotifications(CANHandle_t xCAN) {
  return HAL_CAN_ActivateNotification(xCAN, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_ALL | CAN_IT_RX_FIFO1_ALL |
                                                (CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK) | CAN_IT_ERROR_ALL);
}

#endif // ifdef HAL_CAN_MODULE_ENABLED
