// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_can_cb.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "stm32xx_registered_can.h"

static List_t xTxMailboxCompleteForCAN[stm32xx_canMAX_INSTANCES][stm32xx_canTX_MAILBOX_MAX_INSTANCES];
static List_t xTxMailboxAbortForCAN[stm32xx_canMAX_INSTANCES][stm32xx_canTX_MAILBOX_MAX_INSTANCES];
static List_t xRxFifoMsgPendingForCAN[stm32xx_canMAX_INSTANCES][stm32xx_canRX_FIFO_MAX_INSTANCES];
static List_t xRxFifoFullForCAN[stm32xx_canMAX_INSTANCES][stm32xx_canRX_FIFO_MAX_INSTANCES];
static List_t xSleepForCAN[stm32xx_canMAX_INSTANCES];
static List_t xWakeupFromRxMsgForCAN[stm32xx_canMAX_INSTANCES];
static List_t xErrorForCAN[stm32xx_canMAX_INSTANCES];

/*!
 * \brief Initialises handler lists.
 * Runs at constructor time, before main. List initialisation requires no
 * additional services and therefore proves safe to run early in the start-up
 * sequence.
 */
static void __attribute__((constructor)) prvCANListInitialise() {
  for (size_t xCardinal = 0U; xCardinal < stm32xx_canMAX_INSTANCES; xCardinal++) {
    for (uint8_t ucTxMailbox = 0U; ucTxMailbox < stm32xx_canTX_MAILBOX_MAX_INSTANCES; ucTxMailbox++) {
      vListInitialise(xTxMailboxCompleteForCAN[xCardinal] + ucTxMailbox);
      vListInitialise(xTxMailboxAbortForCAN[xCardinal] + ucTxMailbox);
    }
    for (uint8_t ucRxFifo = 0U; ucRxFifo < stm32xx_canRX_FIFO_MAX_INSTANCES; ucRxFifo++) {
      vListInitialise(xRxFifoMsgPendingForCAN[xCardinal] + ucRxFifo);
      vListInitialise(xRxFifoFullForCAN[xCardinal] + ucRxFifo);
    }
    vListInitialise(&xSleepForCAN[xCardinal]);
    vListInitialise(&xWakeupFromRxMsgForCAN[xCardinal]);
    vListInitialise(&xErrorForCAN[xCardinal]);
  }
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox0Complete(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox0CompleteCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteForCAN[xRegisteredCardinalOfCAN(xCAN)] + 0U, xTxMailboxComplete);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox1Complete(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox1CompleteCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteForCAN[xRegisteredCardinalOfCAN(xCAN)] + 1U, xTxMailboxComplete);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox2Complete(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox2CompleteCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxComplete(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 2U);
    return pdPASS;
  }
  vListYield(xTxMailboxCompleteForCAN[xRegisteredCardinalOfCAN(xCAN)] + 2U, xTxMailboxComplete);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox0Abort(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox0AbortCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortForCAN[xRegisteredCardinalOfCAN(xCAN)] + 0U, xTxMailboxAbort);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox1Abort(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox1AbortCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortForCAN[xRegisteredCardinalOfCAN(xCAN)] + 1U, xTxMailboxAbort);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvTxMailbox2Abort(CANHandle_t xCAN)
#else
void HAL_CAN_TxMailbox2AbortCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xTxMailboxAbort(void *pxOwner, TickType_t xValue) {
    ((CANTxMailboxHandler_t)pxOwner)(xCAN, 2U);
    return pdPASS;
  }
  vListYield(xTxMailboxAbortForCAN[xRegisteredCardinalOfCAN(xCAN)] + 2U, xTxMailboxAbort);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvRxFifo0MsgPending(CANHandle_t xCAN)
#else
void HAL_CAN_RxFifo0MsgPendingCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xRxFifoMsgPendingForCAN[xRegisteredCardinalOfCAN(xCAN)] + 0U, xRxFifoMsgPending);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvRxFifo1MsgPending(CANHandle_t xCAN)
#else
void HAL_CAN_RxFifo1MsgPendingCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xRxFifoMsgPending(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xRxFifoMsgPendingForCAN[xRegisteredCardinalOfCAN(xCAN)] + 1U, xRxFifoMsgPending);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvRxFifo0Full(CANHandle_t xCAN)
#else
void HAL_CAN_RxFifo0FullCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 0U);
    return pdPASS;
  }
  vListYield(xRxFifoFullForCAN[xRegisteredCardinalOfCAN(xCAN)] + 0U, xRxFifoFull);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvRxFifo1Full(CANHandle_t xCAN)
#else
void HAL_CAN_RxFifo1FullCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xRxFifoFull(void *pxOwner, TickType_t xValue) {
    ((CANRxFifoHandler_t)pxOwner)(xCAN, 1U);
    return pdPASS;
  }
  vListYield(xRxFifoFullForCAN[xRegisteredCardinalOfCAN(xCAN)] + 1U, xRxFifoFull);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvSleep(CANHandle_t xCAN)
#else
void HAL_CAN_SleepCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xSleep(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xSleepForCAN[xRegisteredCardinalOfCAN(xCAN)], xSleep);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvWakeupFromRxMsg(CANHandle_t xCAN)
#else
void HAL_CAN_WakeUpFromRxMsgCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xWakeUpFromRxMsg(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xWakeupFromRxMsgForCAN[xRegisteredCardinalOfCAN(xCAN)], xWakeUpFromRxMsg);
}

#if USE_HAL_CAN_REGISTER_CALLBACKS
static void prvError(CANHandle_t xCAN)
#else
void HAL_CAN_ErrorCallback(CANHandle_t xCAN)
#endif
{
  BaseType_t xError(void *pxOwner, TickType_t xValue) {
    ((CANHandler_t)pxOwner)(xCAN);
    return pdPASS;
  }
  vListYield(&xErrorForCAN[xRegisteredCardinalOfCAN(xCAN)], xError);
}

ListItem_t *pxCANRegisterTxMailboxComplete(CANHandle_t xCAN, uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler,
                                           TickType_t xDelay) {
  List_t *pxForCAN = xTxMailboxCompleteForCAN[xRegisteredCardinalOfCAN(xCAN)] + ucTxMailbox;
#if USE_HAL_CAN_REGISTER_CALLBACKS
  /*
   * Assume that the transmit mailbox call-back identifiers run contiguously: 0,
   * 1, 2.
   */
  static void (*prvTxMailboxComplete[])(CANHandle_t xCAN) = {prvTxMailbox0Complete, prvTxMailbox1Complete,
                                                             prvTxMailbox2Complete};
  if (listLIST_IS_EMPTY(pxForCAN))
    HAL_CAN_RegisterCallback(xCAN, HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID + ucTxMailbox, prvTxMailboxComplete[ucTxMailbox]);
#endif
  /*
   * Beware of race conditions.
   *
   *	if (!listLIST_IS_INITIALISED(pxForCAN))
   *		vListInitialise(pxForCAN);
   */
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterTxMailboxAbort(CANHandle_t xCAN, uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler,
                                        TickType_t xDelay) {
  List_t *pxForCAN = xTxMailboxAbortForCAN[xRegisteredCardinalOfCAN(xCAN)] + ucTxMailbox;
#if USE_HAL_CAN_REGISTER_CALLBACKS
  static void (*prvTxMailboxAbort[])(CANHandle_t xCAN) = {prvTxMailbox0Abort, prvTxMailbox1Abort, prvTxMailbox2Abort};
  if (listLIST_IS_EMPTY(pxForCAN))
    HAL_CAN_RegisterCallback(xCAN, HAL_CAN_TX_MAILBOX0_ABORT_CB_ID + ucTxMailbox, prvTxMailboxAbort[ucTxMailbox]);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterRxFifoMsgPending(CANHandle_t xCAN, uint8_t ucRxFifo, CANRxFifoHandler_t xHandler,
                                          TickType_t xDelay) {
  List_t *pxForCAN = xRxFifoMsgPendingForCAN[xRegisteredCardinalOfCAN(xCAN)] + ucRxFifo;
#if USE_HAL_CAN_REGISTER_CALLBACKS
  /*
   * The receiver FIFO message pending call-back identifiers do not run
   * contiguously as they do for the transmit mailbox call-backs. They skip from
   * 6 to 8, interleaved with the FIFO full identifiers.
   */
  static void (*prvRxFifoMsgPending[])(CANHandle_t xCAN) = {prvRxFifo0MsgPending, prvRxFifo1MsgPending};
  if (listLIST_IS_EMPTY(pxForCAN))
    HAL_CAN_RegisterCallback(xCAN, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID + (ucRxFifo << 1U),
                             prvRxFifoMsgPending[ucRxFifo]);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterRxFifoFull(CANHandle_t xCAN, uint8_t ucRxFifo, CANRxFifoHandler_t xHandler,
                                    TickType_t xDelay) {
  List_t *pxForCAN = xRxFifoMsgPendingForCAN[xRegisteredCardinalOfCAN(xCAN)] + ucRxFifo;
#if USE_HAL_CAN_REGISTER_CALLBACKS
  static void (*prvRxFifoFull[])(CANHandle_t xCAN) = {prvRxFifo0Full, prvRxFifo1Full};
  if (listLIST_IS_EMPTY(pxForCAN))
    HAL_CAN_RegisterCallback(xCAN, HAL_CAN_RX_FIFO0_FULL_CB_ID + (ucRxFifo << 1U), prvRxFifoFull[ucRxFifo]);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterSleep(CANHandle_t xCAN, CANHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForCAN = xSleepForCAN + xRegisteredCardinalOfCAN(xCAN);
#if USE_HAL_CAN_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForCAN)) HAL_CAN_RegisterCallback(xCAN, HAL_CAN_SLEEP_CB_ID, prvSleep);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterWakeupFromRxMsg(CANHandle_t xCAN, CANHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForCAN = xWakeupFromRxMsgForCAN + xRegisteredCardinalOfCAN(xCAN);
#if USE_HAL_CAN_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForCAN)) HAL_CAN_RegisterCallback(xCAN, HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID, prvWakeupFromRxMsg);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

ListItem_t *pxCANRegisterError(CANHandle_t xCAN, CANHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForCAN = xErrorForCAN + xRegisteredCardinalOfCAN(xCAN);
#if USE_HAL_CAN_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForCAN)) HAL_CAN_RegisterCallback(xCAN, HAL_CAN_ERROR_CB_ID, prvError);
#endif
  return pxListInsertNew(pxForCAN, xHandler, xDelay);
}

void vCANUnregister(ListItem_t *pxRegisteredForCAN) {
  (void)uxListRemove(pxRegisteredForCAN);
  vPortFree(pxRegisteredForCAN);
}

#endif // ifdef HAL_CAN_MODULE_ENABLED
