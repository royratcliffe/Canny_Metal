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

void vCANTxDataInit(struct CANTx *pxCANTx) {
  vCANTxInit(pxCANTx);
  vCANTxData(pxCANTx);
}

void vCANTxData(struct CANTx *pxCANTx) { pxCANTx->xTxHeader.RTR = CAN_RTR_DATA; }

void vCANTxRemote(struct CANTx *pxCANTx) { pxCANTx->xTxHeader.RTR = CAN_RTR_REMOTE; }

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

/*!
 * \brief Error fields to \c NULL terminated vector of string pointers.
 *
 * \param[in] ulError Error bits.
 *
 * \returns Answers a dynamically-allocated vector of error string pointers terminated by a \c NULL string pointer. Free
 * the non-\c NULL return vector using \c pvPortFree. Answers \c NULL if no errors.
 *
 * First, quickly count the number of bits in the error word. Reduces the
 * number of re-allocations required.
 */
const char **pcCANErrorStrs(uint32_t ulError) {
  static struct {
    uint32_t ulError;
    const char *pcStr;
  } const sErrorAndStr[] = {
      {HAL_CAN_ERROR_EWG, "Protocol Error Warning"},
      {HAL_CAN_ERROR_EPV, "Error Passive"},
      {HAL_CAN_ERROR_BOF, "Bus-off error"},
      {HAL_CAN_ERROR_STF, "Stuff error"},
      {HAL_CAN_ERROR_FOR, "Form error"},
      {HAL_CAN_ERROR_ACK, "Acknowledgement error"},
      {HAL_CAN_ERROR_BR, "Bit recessive error"},
      {HAL_CAN_ERROR_BD, "Bit dominant error"},
      {HAL_CAN_ERROR_CRC, "CRC error"},
      {HAL_CAN_ERROR_RX_FOV0, "Rx FIFO0 overrun error"},
      {HAL_CAN_ERROR_RX_FOV1, "Rx FIFO1 overrun error"},
      {HAL_CAN_ERROR_TX_ALST0, "TxMailbox 0 transmit failure due to arbitration lost"},
      {HAL_CAN_ERROR_TX_TERR0, "TxMailbox 0 transmit failure due to transmit error"},
      {HAL_CAN_ERROR_TX_ALST1, "TxMailbox 1 transmit failure due to arbitration lost"},
      {HAL_CAN_ERROR_TX_TERR1, "TxMailbox 1 transmit failure due to transmit error"},
      {HAL_CAN_ERROR_TX_ALST2, "TxMailbox 2 transmit failure due to arbitration lost"},
      {HAL_CAN_ERROR_TX_TERR2, "TxMailbox 2 transmit failure due to transmit error"},
      {HAL_CAN_ERROR_TIMEOUT, "Timeout error"},
      {HAL_CAN_ERROR_NOT_INITIALIZED, "Peripheral not initialized"},
      {HAL_CAN_ERROR_NOT_READY, "Peripheral not ready"},
      {HAL_CAN_ERROR_NOT_STARTED, "Peripheral not started"},
      {HAL_CAN_ERROR_PARAM, "Parameter error"},
  };

  uint8_t xErrorCount = ucCount32Bits(ulError);
  if (xErrorCount == 0U) return NULL;
  const char **ppcErrorStrs = pvPortMalloc(sizeof(*ppcErrorStrs) * xErrorCount + 1U);
  configASSERT(ppcErrorStrs);
  size_t xErrorStrs = 0U;
  for (size_t xErrorAndStr = 0U; xErrorAndStr < DIM_OF(sErrorAndStr); xErrorAndStr++)
    if (ulError & sErrorAndStr[xErrorAndStr].ulError) ppcErrorStrs[xErrorStrs++] = sErrorAndStr[xErrorAndStr].pcStr;
  ppcErrorStrs[xErrorStrs] = NULL;
  return ppcErrorStrs;
}

#endif // ifdef HAL_CAN_MODULE_ENABLED
