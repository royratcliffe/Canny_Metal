/*
 * stm32xx_uart_cb.c
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "stm32xx_uart_cb.h"

#include "stm32xx_registered_uart.h"

#include "list_ex.h"

static List_t xTxHalfCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xTxCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxHalfCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxCpltForUART[stm32xx_uartMAX_INSTANCES];
static List_t xRxEventForUART[stm32xx_uartMAX_INSTANCES];

static void __attribute__((constructor)) prvListInitialise() {
  for (size_t xCardinal = 0U; xCardinal < stm32xx_uartMAX_INSTANCES; xCardinal++) {
    vListInitialise(xTxHalfCpltForUART + xCardinal);
    vListInitialise(xTxCpltForUART + xCardinal);
    vListInitialise(xRxHalfCpltForUART + xCardinal);
    vListInitialise(xRxCpltForUART + xCardinal);
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

void vUARTUnregisterRxEvent(ListItem_t *pxRxEvent) {
  (void)uxListRemove(pxRxEvent);
  vPortFree(pxRxEvent);
}
