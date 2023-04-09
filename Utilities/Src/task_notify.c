/*
 * task_notify.c (Utilities)
 */

#include "task_notify.h"

/*!
 * \file
 * \brief Useful task notification tools.
 */

#include "task.h"

uint32_t xTaskNotifyWaitExpected(uint32_t ulExpectNotified,
                                 TickType_t xTicksToWait) {
  uint32_t ulActualNotified;
  if (!xTaskNotifyWait(0UL, ulExpectNotified, &ulActualNotified, xTicksToWait))
    return 0UL;
  /*
   * Do not assert the actual verses expected.
   *
   *    configASSERT(ulActualNotified == ulExpectNotified);
   *
   * The match fails in cases where the wait expectations comprise more than one
   * notification. In such cases, the actual always intersects the expection.
   */
  configASSERT(ulActualNotified & ulExpectNotified);
  return ulActualNotified;
}
