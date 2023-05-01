// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "stm32xx_mcu.h"

#include "FreeRTOS.h"

#define stm32xx_canMAX_INSTANCES 2U

#define stm32xx_canTX_MAILBOX_MAX_INSTANCES 3U

#define stm32xx_canRX_FIFO_MAX_INSTANCES 2U

typedef CAN_TypeDef *CAN_t;

#ifdef HAL_CAN_MODULE_ENABLED

typedef CAN_HandleTypeDef *CANHandle_t;

typedef void (*CANTxMailboxHandler_t)(CANHandle_t xCAN, uint32_t ulTxMailbox);

typedef void (*CANRxFifoHandler_t)(CANHandle_t xCAN, uint32_t ulRxFifo);

typedef void (*CANHandler_t)(CANHandle_t xCAN);

HAL_StatusTypeDef xCANConfigFilterFifo0IdMask32Bit(CAN_HandleTypeDef *pxCAN);

/*!
 * \defgroup STM32xx_halCAN_Tx
 * \{
 */

/*!
 * \brief Encapsulates a CAN transmission.
 *
 * The hardware-abstraction layer determines the mailbox automatically.
 */
struct CANTx {
  CAN_TxHeaderTypeDef xTxHeader;
  uint8_t ucTxData[8];
  uint32_t ulTxMailbox;
};

void vCANTxInit(struct CANTx *pxCANTx);

HAL_StatusTypeDef xCANAddTxMessage(CAN_HandleTypeDef *pxCAN, struct CANTx *pxCANTx);

/*!
 * \brief Set up transmit header's standard identifier.
 * \param ulStdId Standard identifier between 0x0U and 0x7ffU inclusively.
 */
static inline void vCANTxHeaderStdId(CAN_TxHeaderTypeDef *pxTxHeader, uint32_t ulStdId) {
  pxTxHeader->StdId = ulStdId;
  pxTxHeader->IDE = CAN_ID_STD;
}

/*!
 * \param ulExtId Extended identifier between 0x0UL anf 0x1fffffffUL inclusively.
 */
static inline void vCANTxHeaderExtId(CAN_TxHeaderTypeDef *pxTxHeader, uint32_t ulExtId) {
  pxTxHeader->ExtId = ulExtId;
  pxTxHeader->IDE = CAN_ID_EXT;
}

static inline void vCANTxStdId(struct CANTx *pxCANTx, uint32_t ulStdId) {
  vCANTxHeaderStdId(&pxCANTx->xTxHeader, ulStdId);
}

static inline void vCANTxExtId(struct CANTx *pxCANTx, uint32_t ulExtId) {
  vCANTxHeaderStdId(&pxCANTx->xTxHeader, ulExtId);
}

/*!
 * \}
 */

/*!
 * \defgroup STM32xx_halCAN_Rx
 * \{
 */

struct CANRx {
  uint32_t ulRxFifo;
  CAN_RxHeaderTypeDef xRxHeader;
  uint8_t ucRxData[8];
};

void vCANRxInit(struct CANRx *pxCANRx, uint32_t ulRxFifo);

HAL_StatusTypeDef xCANGetRxMessage(CAN_HandleTypeDef *pxCAN, struct CANRx *pxCANRx);

BaseType_t xCANRxIsData(struct CANRx *pxCANRx);

BaseType_t xCANRxIsRemote(struct CANRx *pxCANRx);

/*!
 * \brief Does the CAN receive header have a matching standard identifier?
 */
static inline BaseType_t xCANRxHeaderIsStdId(const CAN_RxHeaderTypeDef *pxRxHeader, uint32_t ulStdId) {
  return pxRxHeader->IDE == CAN_ID_STD && pxRxHeader->StdId == ulStdId;
}

static inline BaseType_t xCANRxIsStdID(const struct CANRx *pxCANRx, uint32_t ulStdId) {
  return xCANRxHeaderIsStdId(&pxCANRx->xRxHeader, ulStdId);
}

/*!
 * \}
 */

#endif // ifdef HAL_CAN_MODULE_ENABLED
