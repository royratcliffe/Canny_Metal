/*
 * msg_ext_task.c
 */

#include "msg_ext_task.h"

#include <memory.h>

BaseType_t xMsgBindTask(MsgBindingHandle_t xMsgBinding, TaskHandle_t xTask) {
  return xMsgBindExtWithBody(xMsgBinding, msgEXT_TYPE_TASK, &xTask, sizeof(xTask));
}

BaseType_t xMsgUnifyTask(MsgUnifierHandle_t xMsgUnifier, TaskHandle_t *pxTask) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnifyExtType(xMsgUnifier, msgEXT_TYPE_TASK, &pvExt, &xExtLengthBytes) || xExtLengthBytes != sizeof(*pxTask))
    return pdFAIL;
  if (pxTask) (void)memcpy(pxTask, pvExt, sizeof(*pxTask));
  return pdPASS;
}
