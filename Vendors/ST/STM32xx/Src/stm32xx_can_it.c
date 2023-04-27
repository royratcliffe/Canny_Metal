/*
 * stm32xx_can_it.c
 */

#include "stm32xx_can_it.h"

HAL_StatusTypeDef xCANActivateAllNotifications(CANHandle_t xCAN) {
  return HAL_CAN_ActivateNotification(xCAN, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_ALL | CAN_IT_RX_FIFO1_ALL |
                                                   (CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK) | CAN_IT_ERROR_ALL);
}
