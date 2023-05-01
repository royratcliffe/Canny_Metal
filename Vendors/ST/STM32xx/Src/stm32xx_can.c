// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_can.h"

#ifdef HAL_CAN_MODULE_ENABLED

#  include "task.h"

// standard library
#  include <string.h>

HAL_StatusTypeDef xCANConfigFilterFifo0IdMask32Bit(CAN_HandleTypeDef *pxCAN) {
  CAN_FilterTypeDef CANFilter = {.FilterIdHigh = 0x0000U,
                                 .FilterIdLow = 0x0000U,
                                 .FilterMaskIdHigh = 0x0000U,
                                 .FilterMaskIdLow = 0x0000U,
                                 .FilterFIFOAssignment = CAN_FILTER_FIFO0,
                                 .FilterBank = 0,
                                 .FilterMode = CAN_FILTERMODE_IDMASK,
                                 .FilterScale = CAN_FILTERSCALE_32BIT,
                                 .FilterActivation = CAN_FILTER_ENABLE,
                                 .SlaveStartFilterBank = 14};
  return HAL_CAN_ConfigFilter(pxCAN, &CANFilter);
}

void vCANTxInit(struct CANTx *pxCANTx) { (void)memset(pxCANTx, 0, sizeof(*pxCANTx)); }

HAL_StatusTypeDef xCANAddTxMessage(CAN_HandleTypeDef *pxCAN, struct CANTx *pxCANTx) {
  return HAL_CAN_AddTxMessage(pxCAN, &pxCANTx->xTxHeader, pxCANTx->ucTxData, &pxCANTx->ulTxMailbox);
}

void vCANRxInit(struct CANRx *pxCANRx, uint32_t ulRxFifo) {
  (void)memset(pxCANRx, 0, sizeof(*pxCANRx));
  pxCANRx->ulRxFifo = ulRxFifo;
}

/*
 * Asserts the \c ulRxFifo member. It should be one of \c CAN_RX_FIFO0 or \c CAN_RX_FIFO1.
 */
HAL_StatusTypeDef xCANGetRxMessage(CAN_HandleTypeDef *pxCAN, struct CANRx *pxCANRx) {
  configASSERT(IS_CAN_RX_FIFO(pxCANRx->ulRxFifo));
  return HAL_CAN_GetRxMessage(pxCAN, pxCANRx->ulRxFifo, &pxCANRx->xRxHeader, pxCANRx->ucRxData);
}

BaseType_t xCANRxIsData(struct CANRx *pxCANRx) { return pxCANRx->xRxHeader.RTR == CAN_RTR_DATA; }

BaseType_t xCANRxIsRemote(struct CANRx *pxCANRx) { return pxCANRx->xRxHeader.RTR == CAN_RTR_REMOTE; }

#endif // ifdef HAL_CAN_MODULE_ENABLED
