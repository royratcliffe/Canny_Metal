/*
 * stm32xx_can_handlers.c
 */

#include "stm32xx_can_cb.h"

static List_t xTxMailboxCompleteHandlers[stm32xx_canTX_MAILBOX_MAX_INSTANCES];
static List_t xTxMailboxAbortHandlers[stm32xx_canTX_MAILBOX_MAX_INSTANCES];
static List_t xRxFifoMsgPendingHandlers[stm32xx_canRX_FIFO_MAX_INSTANCES];
static List_t xRxFifoFullHandlers[stm32xx_canRX_FIFO_MAX_INSTANCES];
static List_t xSleepHandlers;
static List_t xWakeUpFromRxMsgHandlers;
static List_t xErrorHandlers;

/*!
 * \brief Initialises handler lists.
 * Runs at constructor time, before main. List initialisation requires no
 * additional services and therefore proves safe to run early in the start-up
 * sequence.
 */
static void __attribute__((constructor)) prvCANListInitialise() {
  for (uint8_t ucTxMailbox = 0U; ucTxMailbox < stm32xx_canTX_MAILBOX_MAX_INSTANCES; ucTxMailbox++) {
    vListInitialise(xTxMailboxCompleteHandlers + ucTxMailbox);
    vListInitialise(xTxMailboxAbortHandlers + ucTxMailbox);
  }
  for (uint8_t ucRxFifo = 0U; ucRxFifo < stm32xx_canRX_FIFO_MAX_INSTANCES; ucRxFifo++) {
    vListInitialise(xRxFifoMsgPendingHandlers + ucRxFifo);
    vListInitialise(xRxFifoFullHandlers + ucRxFifo);
  }
  vListInitialise(&xSleepHandlers);
  vListInitialise(&xWakeUpFromRxMsgHandlers);
  vListInitialise(&xErrorHandlers);
}

ListItem_t *pxCANRegisterTxMailboxComplete(uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler, TickType_t xDelay) {
  /*
   * Beware of race conditions.
   *
   *	if (!listLIST_IS_INITIALISED(&xTxMailbox0CompleteHandlers))
   *		vListInitialise(&xTxMailbox0CompleteHandlers);
   */
  return pxListInsertNew(xTxMailboxCompleteHandlers + ucTxMailbox, xHandler, xDelay);
}

ListItem_t *pxCANRegisterTxMailboxAbort(uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(xTxMailboxAbortHandlers + ucTxMailbox, xHandler, xDelay);
}

ListItem_t *pxCANRegisterRxFifoMsgPending(uint8_t ucRxFifo, CANRxFifoHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(xRxFifoMsgPendingHandlers + ucRxFifo, xHandler, xDelay);
}

ListItem_t *pxCANRegisterRxFifoFull(uint8_t ucRxFifo, CANRxFifoHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(xRxFifoFullHandlers + ucRxFifo, xHandler, xDelay);
}

ListItem_t *pxCANRegisterSleep(CANHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(&xSleepHandlers, xHandler, xDelay);
}

ListItem_t *pxCANRegisterWakeUpFromRxMsg(CANHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(&xWakeUpFromRxMsgHandlers, xHandler, xDelay);
}

ListItem_t *pxCANRegisterError(CANHandler_t xHandler, TickType_t xDelay) {
  return pxListInsertNew(&xErrorHandlers, xHandler, xDelay);
}

void HAL_CAN_TxMailbox0CompleteCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteHandlers + 0U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox1CompleteCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteHandlers + 1U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox2CompleteCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 2U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteHandlers + 2U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox0AbortCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortHandlers + 0U, xTxMailboxAbort);
}

void HAL_CAN_TxMailbox1AbortCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortHandlers + 1U, xTxMailboxAbort);
}

void HAL_CAN_TxMailbox2AbortCallback(CANHandle_t xCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 2U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortHandlers + 2U, xTxMailboxAbort);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CANHandle_t xCAN) {
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xRxFifoMsgPendingHandlers + 0U, xRxFifoMsgPending);
}

void HAL_CAN_RxFifo0FullCallback(CANHandle_t xCAN) {
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xRxFifoFullHandlers + 0U, xRxFifoFull);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CANHandle_t xCAN) {
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xRxFifoMsgPendingHandlers + 1U, xRxFifoMsgPending);
}

void HAL_CAN_RxFifo1FullCallback(CANHandle_t xCAN) {
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xRxFifoFullHandlers + 1U, xRxFifoFull);
}

void HAL_CAN_SleepCallback(CANHandle_t xCAN) {
  BaseType_t xSleep(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xSleepHandlers, xSleep);
}

void HAL_CAN_WakeUpFromRxMsgCallback(CANHandle_t xCAN) {
  BaseType_t xWakeUpFromRxMsg(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xWakeUpFromRxMsgHandlers, xWakeUpFromRxMsg);
}

void HAL_CAN_ErrorCallback(CANHandle_t xCAN) {
  BaseType_t xError(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xErrorHandlers, xError);
}
