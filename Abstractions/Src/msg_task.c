/*
 * msg_task.c
 */

#include "msg_task.h"

#include <memory.h>

/*!
 * \brief Packs a task handle.
 *
 * Uses the \c msgEXT_TYPE_TASK extension to pack a task handle.
 */
BaseType_t xMsgPackTask(MsgPackHandle_t xMsgPack, TaskHandle_t xTask) {
  return xMsgPackExtWithBody(xMsgPack, &xTask, sizeof(xTask), msgEXT_TYPE_TASK);
}

/*!
 * The task handle *could* be \c NULL.
 */
BaseType_t xMsgUnpackTask(MsgUnpackHandle_t xMsgUnpack, TaskHandle_t *pxTask) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnpackExtType(xMsgUnpack, &pvExt, &xExtLengthBytes,
                         msgEXT_TYPE_TASK) ||
      xExtLengthBytes != sizeof(*pxTask))
    return pdFAIL;
  (void)memcpy(pxTask, pvExt, sizeof(*pxTask));
  return pdPASS;
}
