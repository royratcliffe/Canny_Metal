// stm32xx_can_cb.h

/*!
 * \file
 *
 * Why call the functions "handlers" rather than "callbacks?"
 * The differing term attempts to make a distinction between the heavy
 * driver-level callback and a lighter handling function, or functions since
 * there can be more than one.
 */

#pragma once

#include "stm32xx_can.h"

#include "list_ex.h"

#ifdef HAL_CAN_MODULE_ENABLED

/*!
 * \param xDelay Ascending priority order of handler invocation. Use
 * \c portMAX_DELAY to add the given handler to the end of the handler list.
 */
ListItem_t *pxCANRegisterTxMailboxComplete(uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterTxMailboxAbort(uint8_t ucTxMailbox, CANTxMailboxHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterRxFifoMsgPending(uint8_t ucRxFifo, CANRxFifoHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterRxFifoFull(uint8_t ucRxFifo, CANRxFifoHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterSleep(CANHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterWakeUpFromRxMsg(CANHandler_t xHandler, TickType_t xDelay);
ListItem_t *pxCANRegisterError(CANHandler_t xHandler, TickType_t xDelay);

#endif // ifdef HAL_CAN_MODULE_ENABLED
