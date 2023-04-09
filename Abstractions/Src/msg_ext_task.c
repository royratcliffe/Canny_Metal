/*
 * msg_ext_task.c
 */

#include <memory.h>
#include <msg_ext_task.h>

BaseType_t xMsgBindTask(MsgBindingHandle_t xMsgBinding, TaskHandle_t xTask) {
  return xMsgBindExtWithBody(xMsgBinding, &xTask, sizeof(xTask), msgEXT_TYPE_TASK);
}

BaseType_t xMsgUnifyTask(MsgUnifierHandle_t xMsgUnpack, TaskHandle_t *pxTask) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnifyExtType(xMsgUnpack, &pvExt, &xExtLengthBytes, msgEXT_TYPE_TASK) || xExtLengthBytes != sizeof(*pxTask))
    return pdFAIL;
  if (pxTask) (void)memcpy(pxTask, pvExt, sizeof(*pxTask));
  return pdPASS;
}
