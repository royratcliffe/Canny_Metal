/*
 * task_notify.h (Utilities)
 */

#pragma once

#include "FreeRTOS.h"

/*!
 * \brief Waits for an expected task notification.
 * \param[in] ulExpectNotified Expected notification, or notifications.
 * \param[in] xTicksToWait Ticks to wait before timing out.
 * \returns \c 0UL if the expected notification does \e not arrive within the
 * time limit. Otherwise answers the actual notification, nor notification set
 * where more than one.
 *
 * Waits for an expected notification only. Ignore any other pending
 * notifications. Enclosing contexts will pick up other notifications \e after
 * the expectation succeeds, e.g. a transfer completes if used for synchronisation
 * with some asynchronous transfer; thus effectively serialising a
 * interrupt-triggered process.
 */
uint32_t xTaskNotifyWaitExpected(uint32_t ulExpectNotified,
                                 TickType_t xTicksToWait);
