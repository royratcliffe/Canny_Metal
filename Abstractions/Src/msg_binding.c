/*
 * msg_binding.c
 */

#include "msg_binding.h"

/*!
 * \file
 *
 * This implementation utilises standard C Message Pack using standard-library
 * memory-allocated string buffers. It links with the full MsgPack library
 * though only invokes a much smaller subset.
 */

#include <msgpack.h>

#include "task.h"

/*!
 * \addtogroup MsgBinding
 * \{
 */

struct MsgBinding {
  msgpack_sbuffer xBuffer;
  msgpack_packer xPacker;
};

MsgBindingHandle_t xMsgBindingNew() {
  MsgBindingHandle_t xMsgBinding = pvPortMalloc(sizeof(*xMsgBinding));
  configASSERT(xMsgBinding);
  msgpack_sbuffer_init(&xMsgBinding->xBuffer);
  msgpack_packer_init(&xMsgBinding->xPacker, &xMsgBinding->xBuffer, msgpack_sbuffer_write);
  return xMsgBinding;
}

void vMsgBindingDestroy(MsgBindingHandle_t xMsgBinding) {
  msgpack_sbuffer_destroy(&xMsgBinding->xBuffer);
  vPortFree(xMsgBinding);
}

void vMsgBindingClear(MsgBindingHandle_t xMsgBinding) { msgpack_sbuffer_clear(&xMsgBinding->xBuffer); }

size_t xMsgBindingBuffer(MsgBindingHandle_t xMsgBinding, const void **ppvBuffer) {
  if (ppvBuffer) *ppvBuffer = xMsgBinding->xBuffer.data;
  return xMsgBinding->xBuffer.size;
}

size_t xMsgBindingSend(MsgBindingHandle_t xMsgBinding, MessageBufferHandle_t xMessageBuffer, TickType_t xTicksToWait) {
  return xMessageBufferSend(xMessageBuffer, xMsgBinding->xBuffer.data, xMsgBinding->xBuffer.size, xTicksToWait);
}

BaseType_t xMsgBindBool(MsgBindingHandle_t xMsgBinding, BaseType_t xBool) {
  int (*pxPackFunction)(msgpack_packer *) = xBool ? msgpack_pack_true : msgpack_pack_false;
  /*
   * A zero return code indicates success. Translate success to pdPASS.
   */
  return pxPackFunction(&xMsgBinding->xPacker) == 0;
}

BaseType_t xMsgBindInt(MsgBindingHandle_t xMsgBinding, int i) {
  return msgpack_pack_int(&xMsgBinding->xPacker, i) == 0;
}

BaseType_t xMsgBindUInt(MsgBindingHandle_t xMsgBinding, unsigned int u) {
  return msgpack_pack_unsigned_int(&xMsgBinding->xPacker, u) == 0;
}

BaseType_t xMsgBindFloat(MsgBindingHandle_t xMsgBinding, float f) {
  return msgpack_pack_float(&xMsgBinding->xPacker, f) == 0;
}

BaseType_t xMsgBindStrWithBody(MsgBindingHandle_t xMsgBinding, const char *pcStr, size_t xStrLengthBytes) {
  return msgpack_pack_str_with_body(&xMsgBinding->xPacker, pcStr, xStrLengthBytes) == 0;
}

BaseType_t xMsgBindStr(MsgBindingHandle_t xMsgBinding, const char *pcStr) {
  return xMsgBindStrWithBody(xMsgBinding, pcStr, strlen(pcStr));
}

BaseType_t xMsgBindExtWithBody(MsgBindingHandle_t xMsgBinding, const void *pvExt, size_t xExtLengthBytes,
                               int8_t cType) {
  return msgpack_pack_ext_with_body(&xMsgBinding->xPacker, pvExt, xExtLengthBytes, cType) == 0;
}

/*!
 * \}
 */
