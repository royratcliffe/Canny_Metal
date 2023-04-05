/*
 * mailbox_msg.c
 */

#include "mailbox_msg.h"
#include "mailbox_prv.h"

#include <memory.h>

size_t xMailboxSendMsg(MailboxHandle_t xMailbox, MsgPackHandle_t xMsgPack,
                       TickType_t xTicksToWait) {
  return xMsgPackSendMessage(xMsgPack, xMailbox->xMessageBuffer, xTicksToWait);
}

BaseType_t xMailboxReceiveMsg(MailboxHandle_t xMailbox,
                              MsgUnpackHandle_t xMsgUnpack,
                              TickType_t xTicksToWait) {
  return xMsgUnpackReceiveMessage(xMsgUnpack, xMailbox->xMessageBuffer,
                                  xTicksToWait);
}

BaseType_t xMsgPackMailbox(MsgPackHandle_t xMsgPack, MailboxHandle_t xMailbox) {
  return xMsgPackExtWithBody(xMsgPack, &xMailbox, sizeof(xMailbox),
                             msgEXT_TYPE_MAILBOX);
}

BaseType_t xMsgUnpackMailbox(MsgUnpackHandle_t xMsgUnpack,
                             MailboxHandle_t *pxMailbox) {
  const void *pvExt;
  size_t xExtLengthBytes;
  if (!xMsgUnpackExtType(xMsgUnpack, &pvExt, &xExtLengthBytes,
                         msgEXT_TYPE_MAILBOX) ||
      xExtLengthBytes != sizeof(*pxMailbox))
    return pdFAIL;
  (void)memcpy(pxMailbox, pvExt, sizeof(*pxMailbox));
  return pdPASS;
}
