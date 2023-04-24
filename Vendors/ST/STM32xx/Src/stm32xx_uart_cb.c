// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_uart_cb.h"

#include "stm32xx_registered_uart.h"

#include "list_ex.h"

static List_t xTxHalfCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xTxCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxHalfCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xErrorForUART[stm32xx_uartMAX_INSTANCES];
static List_t xAbortCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xAbortTransmitCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xAbortReceiveCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xWakeupForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxEventForUART[stm32xx_uartMAX_INSTANCES];

static void __attribute__((constructor)) prvListInitialise() {
  for (size_t xCardinal = 0U; xCardinal < stm32xx_uartMAX_INSTANCES; xCardinal++) {
    vListInitialise(xTxHalfCpltForUART + xCardinal);
    vListInitialise(xTxCpltForUART + xCardinal);
    vListInitialise(xRxHalfCpltForUART + xCardinal);
    vListInitialise(xRxCpltForUART + xCardinal);
    vListInitialise(xErrorForUART + xCardinal);
    vListInitialise(xAbortCpltForUART + xCardinal);
    vListInitialise(xAbortTransmitCpltForUART + xCardinal);
    vListInitialise(xAbortReceiveCpltForUART + xCardinal);
    vListInitialise(xWakeupForUART + xCardinal);
    vListInitialise(xRxEventForUART + xCardinal);
  }
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvTxHalfCplt(UARTHandle_t xUART)
#else
void HAL_UART_TxHalfCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xTxHalfCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterTxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xTxHalfCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_TX_HALFCOMPLETE_CB_ID, prvTxHalfCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvTxCplt(UARTHandle_t xUART)
#else
void HAL_UART_TxCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xTxCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterTxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xTxCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_TX_COMPLETE_CB_ID, prvTxCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvRxHalfCplt(UARTHandle_t xUART)
#else
void HAL_UART_RxHalfCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xRxHalfCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterRxHalfCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xRxHalfCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_RX_HALFCOMPLETE_CB_ID, prvRxHalfCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvRxCplt(UARTHandle_t xUART)
#else
void HAL_UART_RxCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xRxCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterRxCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xRxCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_RX_COMPLETE_CB_ID, prvRxCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvError(UARTHandle_t xUART)
#else
void HAL_UART_ErrorCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xErrorForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterError(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xErrorForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_ERROR_CB_ID, prvError);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvAbortCplt(UARTHandle_t xUART)
#else
void HAL_UART_AbortCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xAbortCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterAbortCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xAbortCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_ABORT_COMPLETE_CB_ID, prvAbortCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvAbortTransmitCplt(UARTHandle_t xUART)
#else
void HAL_UART_AbortTransmitCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xAbortTransmitCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterAbortTransmitCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xAbortTransmitCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID, prvAbortTransmitCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvAbortReceiveCplt(UARTHandle_t xUART)
#else
void HAL_UART_AbortReceiveCpltCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xAbortReceiveCpltForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterAbortReceiveCplt(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xAbortReceiveCpltForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID, prvAbortReceiveCplt);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvWakeup(UARTHandle_t xUART)
#else
void HAL_UART_WakeupCallback(UARTHandle_t xUART)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTHandler_t)pxOwner)(xUART);
    return pdPASS;
  }
  vListYield(xWakeupForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterWakeup(UARTHandle_t xUART, UARTHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xWakeupForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterCallback(xUART, HAL_UART_WAKEUP_CB_ID, prvWakeup);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer)
#else
void HAL_UARTEx_RxEventCallback(UARTHandle_t xUART, uint16_t usXfer)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    (void)xValue;
    ((UARTRxEventHandler_t)pxOwner)(xUART, usXfer);
    return pdPASS;
  }
  vListYield(xRxEventForUART + xRegisteredCardinalOfUART(xUART), xYield);
}

ListItem_t *pxUARTRegisterRxEvent(UARTHandle_t xUART, UARTRxEventHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForUART = xRxEventForUART + xRegisteredCardinalOfUART(xUART);
#if USE_HAL_UART_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForUART)) HAL_UART_RegisterRxEventCallback(xUART, prvRxEvent);
#endif
  return pxListInsertNew(pxForUART, xHandler, xDelay);
}

void vUARTUnregister(ListItem_t *pxRegisteredForUART) {
  (void)uxListRemove(pxRegisteredForUART);
  vPortFree(pxRegisteredForUART);
}
