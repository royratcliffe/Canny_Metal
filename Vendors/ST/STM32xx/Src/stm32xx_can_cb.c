/*
 * stm32xx_can_handlers.c
 */

#include "stm32xx_can_cb.h"

static List_t xhalTxMailboxCompleteHandlers[halcan_mailboxMAX_INSTANCES];
static List_t xhalTxMailboxAbortHandlers[halcan_mailboxMAX_INSTANCES];
static List_t xhalRxFifoMsgPendingHandlers[halcan_rxfifoMAX_INSTANCES];
static List_t xhalRxFifoFullHandlers[halcan_rxfifoMAX_INSTANCES];
static List_t xhalSleepHandlers;
static List_t xhalWakeUpFromRxMsgHandlers;
static List_t xhalErrorHandlers;

/*!
 * \brief Initialises handler lists.
 * Runs at constructor time, before main. List initialisation requires no
 * additional services and therefore proves safe to run early in the start-up
 * sequence.
 */
static void __attribute__((constructor)) prvhalCANListInitialise() {
  for (uint8_t ucMailbox = 0U; ucMailbox < halcan_mailboxMAX_INSTANCES; ucMailbox++) {
    vListInitialise(xhalTxMailboxCompleteHandlers + ucMailbox);
    vListInitialise(xhalTxMailboxAbortHandlers + ucMailbox);
  }
  for (uint8_t ucRxFifo = 0U; ucRxFifo < halcan_rxfifoMAX_INSTANCES; ucRxFifo++) {
    vListInitialise(xhalRxFifoMsgPendingHandlers + ucRxFifo);
    vListInitialise(xhalRxFifoFullHandlers + ucRxFifo);
  }
  vListInitialise(&xhalSleepHandlers);
  vListInitialise(&xhalWakeUpFromRxMsgHandlers);
  vListInitialise(&xhalErrorHandlers);
}

ListItem_t *pxhalCANRegisterTxMailboxCompleteHandler(uint8_t ucMailbox, halCANHandler_t xhalCANHandler,
                                                     TickType_t xPriority) {
  /*
   * Beware of race conditions.
   *
   *	if (!listLIST_IS_INITIALISED(&xTxMailbox0CompleteHandlers))
   *		vListInitialise(&xhalTxMailbox0CompleteHandlers);
   */
  return pxListInsertNew(xhalTxMailboxCompleteHandlers + ucMailbox, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterTxMailboxAbortHandler(uint8_t ucMailbox, halCANHandler_t xhalCANHandler,
                                                  TickType_t xPriority) {
  return pxListInsertNew(xhalTxMailboxAbortHandlers + ucMailbox, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterRxFifoMsgPendingHandler(uint8_t ucRxFifo, halCANHandler_t xhalCANHandler,
                                                    TickType_t xPriority) {
  return pxListInsertNew(xhalRxFifoMsgPendingHandlers + ucRxFifo, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterRxFifoFullHandler(uint8_t ucRxFifo, halCANHandler_t xhalCANHandler, TickType_t xPriority) {
  return pxListInsertNew(xhalRxFifoFullHandlers + ucRxFifo, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterSleepHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority) {
  return pxListInsertNew(&xhalSleepHandlers, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterWakeUpFromRxMsgHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority) {
  return pxListInsertNew(&xhalWakeUpFromRxMsgHandlers, xhalCANHandler, xPriority);
}

ListItem_t *pxhalCANRegisterErrorHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority) {
  return pxListInsertNew(&xhalErrorHandlers, xhalCANHandler, xPriority);
}

void HAL_CAN_TxMailbox0CompleteCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 0U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxCompleteHandlers + 0U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox1CompleteCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 1U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxCompleteHandlers + 1U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox2CompleteCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 2U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxCompleteHandlers + 2U, xTxMailboxComplete);
}

void HAL_CAN_TxMailbox0AbortCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 0U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxAbortHandlers + 0U, xTxMailboxAbort);
}

void HAL_CAN_TxMailbox1AbortCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 1U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxAbortHandlers + 1U, xTxMailboxAbort);
}

void HAL_CAN_TxMailbox2AbortCallback(halCANHandle_t xhalCAN) {
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 2U);
    return pdPASS;
  }
  vListYield(xhalTxMailboxAbortHandlers + 2U, xTxMailboxAbort);
}

void HAL_CAN_RxFifo0MsgPendingCallback(halCANHandle_t xhalCAN) {
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 0U);
    return pdPASS;
  }
  vListYield(xhalRxFifoMsgPendingHandlers + 0U, xRxFifoMsgPending);
}

void HAL_CAN_RxFifo0FullCallback(halCANHandle_t xhalCAN) {
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 0U);
    return pdPASS;
  }
  vListYield(xhalRxFifoFullHandlers + 0U, xRxFifoFull);
}

void HAL_CAN_RxFifo1MsgPendingCallback(halCANHandle_t xhalCAN) {
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 1U);
    return pdPASS;
  }
  vListYield(xhalRxFifoMsgPendingHandlers + 1U, xRxFifoMsgPending);
}

void HAL_CAN_RxFifo1FullCallback(halCANHandle_t xhalCAN) {
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN, 1U);
    return pdPASS;
  }
  vListYield(xhalRxFifoFullHandlers + 1U, xRxFifoFull);
}

void HAL_CAN_SleepCallback(halCANHandle_t xhalCAN) {
  BaseType_t xSleep(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN);
    return pdPASS;
  }
  vListYield(&xhalSleepHandlers, xSleep);
}

void HAL_CAN_WakeUpFromRxMsgCallback(halCANHandle_t xhalCAN) {
  BaseType_t xWakeUpFromRxMsg(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN);
    return pdPASS;
  }
  vListYield(&xhalWakeUpFromRxMsgHandlers, xWakeUpFromRxMsg);
}

void HAL_CAN_ErrorCallback(halCANHandle_t xhalCAN) {
  BaseType_t xError(void *pxOwner, TickType_t xValue) {
    ((halCANHandler_t)pxOwner)(xhalCAN);
    return pdPASS;
  }
  vListYield(&xhalErrorHandlers, xError);
}
