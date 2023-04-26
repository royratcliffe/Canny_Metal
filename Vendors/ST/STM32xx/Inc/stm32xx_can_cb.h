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
 * \param xPriority Ascending priority order of handler invocation. Use
 * `portMAX_DELAY` to add the given handler to the end of the handler list.
 */
ListItem_t *pxhalCANRegisterTxMailboxCompleteHandler(uint8_t ucMailbox, halCANHandler_t xhalCANHandler,
                                                     TickType_t xPriority);
ListItem_t *pxhalCANRegisterTxMailboxAbortHandler(uint8_t ucMailbox, halCANHandler_t xhalCANHandler,
                                                  TickType_t xPriority);
ListItem_t *pxhalCANRegisterRxFifoMsgPendingHandler(uint8_t ucRxFifo, halCANHandler_t xhalCANHandler,
                                                    TickType_t xPriority);
ListItem_t *pxhalCANRegisterRxFifoFullHandler(uint8_t ucRxFifo, halCANHandler_t xhalCANHandler, TickType_t xPriority);
ListItem_t *pxhalCANRegisterSleepHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority);
ListItem_t *pxhalCANRegisterWakeUpFromRxMsgHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority);
ListItem_t *pxhalCANRegisterErrorHandler(halCANHandler_t xhalCANHandler, TickType_t xPriority);

#endif // ifdef HAL_CAN_MODULE_ENABLED
