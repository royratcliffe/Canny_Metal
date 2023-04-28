// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "msg_ext_opaque.h"

#include <memory.h>

BaseType_t xMsgBindOpaque(MsgBindingHandle_t xMsgBinding, void *pvOpaque) {
  return xMsgBindExtWithBody(xMsgBinding, msgEXT_TYPE_OPAQUE, &pvOpaque, sizeof(pvOpaque));
}

/*
 * Unifies if the extension type match \e and the length matches that of a void
 * pointer. Mismatches if the size fails to match.
 */
BaseType_t xMsgUnifyOpaque(MsgUnifierHandle_t xMsgUnifier, void **ppvOpaque) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnifyExtType(xMsgUnifier, msgEXT_TYPE_OPAQUE, &pvExt, &xExtLengthBytes) ||
      xExtLengthBytes != sizeof(*ppvOpaque))
    return pdFAIL;
  if (ppvOpaque) (void)memcpy(ppvOpaque, pvExt, sizeof(*ppvOpaque));
  return pdPASS;
}
