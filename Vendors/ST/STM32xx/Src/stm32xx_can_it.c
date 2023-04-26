/*
 * stm32xx_can_it.c
 */

#include "stm32xx_can_it.h"

HAL_StatusTypeDef xhalCANActivateAllNotifications(halCANHandle_t xhalCAN) {
  return HAL_CAN_ActivateNotification(xhalCAN, CAN_IT_TX_MAILBOX_EMPTY | halCAN_IT_RX_FIFO0 | halCAN_IT_RX_FIFO1 |
                                                   (CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK) | halCAN_IT_ERROR);
}
