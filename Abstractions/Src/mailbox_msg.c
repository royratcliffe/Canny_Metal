/*
 * mailbox_msg.c
 */

#include "mailbox_msg.h"
#include "mailbox_prv.h"

#include <memory.h>

#ifndef mailbox_send_msgUSE_MSG_BINDING_SEND
#  define mailbox_send_msgUSE_MSG_BINDING_SEND 0
#endif

BaseType_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding, TickType_t xTicksToWait) {
  if (xMailboxOrSelf(&xMailbox) == pdFAIL) return pdFAIL;
  size_t xBytesSent;
#if mailbox_send_msgUSE_MSG_BINDING_SEND
  xBytesSent = xMsgBindingSend(xMsgBinding, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
#else
  const void *pvBuffer;
  size_t xBufferLengthBytes = xMsgBindingBuffer(xMsgBinding, &pvBuffer);
  xBytesSent = xMailboxSend(xMailbox, pvBuffer, xBufferLengthBytes, xTicksToWait);
#endif
  if (xBytesSent) (void)xMailboxSent(xMailbox);
  return xBytesSent;
}

void vMailboxSendMsg(MailboxHandle_t xMailbox, MsgBindingHandle_t xMsgBinding) {
  if (xMailboxOrSelf(&xMailbox) == pdPASS) (void)xMailboxSendMsg(xMailbox, xMsgBinding, 0U);
}

BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait) {
  if (xMailboxOrSelf(&xMailbox) == pdFAIL) return pdFAIL;
  /*
   * Throw away the size of the message received. The size does not matter. The
   * unifier knows the size. Answer pass or fail.
   */
  return xMsgUnifierReceive(xMsgUnifier, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
}

BaseType_t xMailboxRelayMsg(MailboxHandle_t xMailbox, MsgUnifierHandle_t xMsgUnifier, TickType_t xTicksToWait) {
  if (xMailboxOrSelf(&xMailbox) == pdFAIL) return pdFAIL;
  return xMsgUnifierRelay(xMsgUnifier, xMailbox->xMessageBuffer, xTicksToWait) != 0UL;
}

BaseType_t xMsgBindMailbox(MsgBindingHandle_t xMsgPack, MailboxHandle_t xMailbox) {
  if (xMailboxOrSelf(&xMailbox) == pdFAIL) return pdFAIL;
  return xMsgBindExtWithBody(xMsgPack, msgEXT_TYPE_MAILBOX, &xMailbox, sizeof(xMailbox));
}

BaseType_t xMsgUnifyMailbox(MsgUnifierHandle_t xMsgUnpack, MailboxHandle_t *pxMailbox) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnifyExtType(xMsgUnpack, msgEXT_TYPE_MAILBOX, &pvExt, &xExtLengthBytes) ||
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
