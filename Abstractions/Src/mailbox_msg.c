/*
 * mailbox_msg.c
 */

#include "mailbox_msg.h"
#include "mailbox_prv.h"

#include <memory.h>

BaseType_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding, TickType_t xTicksToWait) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  return xMsgBindingSend(xMsgBinding, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
}

BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  /*
   * Throw away the size of the message received. The size does not matter. The
   * unifier knows the size. Answer pass or fail.
   */
  return xMsgUnifierReceive(xMsgUnifier, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
}

BaseType_t xMailboxRelayMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  return xMsgUnifierRelay(xMsgUnifier, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
}

BaseType_t xMsgBindMailbox(MsgBindingHandle_t xMsgPack, MailboxHandle_t xMailbox) {
  if (xMailbox == NULL && (xMailbox = xMailboxSelf()) == NULL) return pdFAIL;
  return xMsgBindExtWithBody(xMsgPack, &xMailbox, sizeof(xMailbox), msgEXT_TYPE_MAILBOX);
}

BaseType_t xMsgUnifyMailbox(MsgUnifierHandle_t xMsgUnpack, MailboxHandle_t *pxMailbox) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnifyExtType(xMsgUnpack, &pvExt, &xExtLengthBytes, msgEXT_TYPE_MAILBOX) ||
      xExtLengthBytes != sizeof(*pxMailbox))
    return pdFAIL;
  if (pxMailbox) (void)memcpy(pxMailbox, pvExt, sizeof(*pxMailbox));
  return pdPASS;
}

/*
 * Is this a good idea? Failure means either of two things: either no unpack
 * next success, or mailbox extension type failure. The interface has no way to
 * discern the difference.
 */
BaseType_t xMsgUnifierNextMailbox(MsgUnifierHandle_t xMsgUnifier, MailboxHandle_t *pxMailbox) {
  return xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyMailbox(xMsgUnifier, pxMailbox);
}
