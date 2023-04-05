/*
 * msg.c
 */

#include "msg.h"

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
 * \addtogroup MsgPack
 * \{
 */

struct MsgPack {
  msgpack_sbuffer xBuffer;
  msgpack_packer xPacker;
};

/*!
 * \brief Creates an new initialised message packer.
 */
MsgPackHandle_t xMsgPackNew() {
  MsgPackHandle_t xMsgPack = pvPortMalloc(sizeof(*xMsgPack));
  configASSERT(xMsgPack);
  msgpack_sbuffer_init(&xMsgPack->xBuffer);
  msgpack_packer_init(&xMsgPack->xPacker, &xMsgPack->xBuffer,
                      msgpack_sbuffer_write);
  return xMsgPack;
}

void vMsgPackDestroy(MsgPackHandle_t xMsgPack) {
  msgpack_sbuffer_destroy(&xMsgPack->xBuffer);
  vPortFree(xMsgPack);
}

void vMsgPackClear(MsgPackHandle_t xMsgPack) {
  msgpack_sbuffer_clear(&xMsgPack->xBuffer);
}

BaseType_t xMsgPackBool(MsgPackHandle_t xMsgPack, BaseType_t xValue) {
  int (*pxPackFunction)(msgpack_packer *) =
      xValue ? msgpack_pack_true : msgpack_pack_false;
  return pxPackFunction(&xMsgPack->xPacker) == 0;
}

BaseType_t xMsgPackFloat(MsgPackHandle_t xMsgPack, float lValue) {
  return msgpack_pack_float(&xMsgPack->xPacker, lValue) == 0;
}

/*!
 * The body should have UTF-8 encoding.
 */
BaseType_t xMsgPackStrWithBody(MsgPackHandle_t xMsgPack, const char *pcStr,
                               size_t xStrLengthBytes) {
  return msgpack_pack_str_with_body(&xMsgPack->xPacker, pcStr,
                                    xStrLengthBytes) == 0;
}

BaseType_t xMsgPackStr(MsgPackHandle_t xMsgPack, const char *pcStr) {
  return xMsgPackStrWithBody(xMsgPack, pcStr, strlen(pcStr));
}

BaseType_t xMsgPackExtWithBody(MsgPackHandle_t xMsgPack, const void *pvExt,
                               size_t xExtLengthBytes, int8_t cType) {
  return msgpack_pack_ext_with_body(&xMsgPack->xPacker, pvExt, xExtLengthBytes,
                                    cType) == 0;
}

size_t xMsgPackSendMessage(MsgPackHandle_t xMsgPack,
                           MessageBufferHandle_t xMessageBuffer,
                           TickType_t xTicksToWait) {
  return xMessageBufferSend(xMessageBuffer, xMsgPack->xBuffer.data,
                            xMsgPack->xBuffer.size, xTicksToWait);
}

/*!
 * \}
 */

/*!
 * \addtogroup MsgUnpack
 * \{
 */

struct MsgUnpack {
  msgpack_unpacker xUnpacker;
  msgpack_unpacked xUnpacked;
};

MsgUnpackHandle_t xMsgUnpackCreate(size_t xInitialBufferSizeBytes) {
  MsgUnpackHandle_t xMsgUnpack = pvPortMalloc(sizeof(*xMsgUnpack));
  configASSERT(xMsgUnpack);
  msgpack_unpacker_init(&xMsgUnpack->xUnpacker, xInitialBufferSizeBytes);
  msgpack_unpacked_init(&xMsgUnpack->xUnpacked);
  return xMsgUnpack;
}

MsgUnpackHandle_t xMsgUnpackNew() {
  return xMsgUnpackCreate(msgINITIAL_BUFFER_SIZE_BYTES);
}

void vMsgUnpackDestroy(MsgUnpackHandle_t xMsgUnpack) {
  msgpack_unpacked_destroy(&xMsgUnpack->xUnpacked);
  msgpack_unpacker_destroy(&xMsgUnpack->xUnpacker);
}

/*!
 * \returns Answers zero if the unpacker fails to reserve sufficient memory to
 * hold the incoming message.
 *
 * Receives zero or one message from the given message buffer.
 */
size_t xMsgUnpackReceiveMessage(MsgUnpackHandle_t xMsgUnpack,
                                MessageBufferHandle_t xMessageBuffer,
                                TickType_t xTicksToWait) {
  size_t xBufferLengthBytes = xMessageBufferNextLengthBytes(xMessageBuffer);
  if (!msgpack_unpacker_reserve_buffer(&xMsgUnpack->xUnpacker,
                                       xBufferLengthBytes))
    return 0UL;
  size_t xReceivedBytes = xMessageBufferReceive(
      xMessageBuffer, msgpack_unpacker_buffer(&xMsgUnpack->xUnpacker),
      xBufferLengthBytes, xTicksToWait);
  msgpack_unpacker_buffer_consumed(&xMsgUnpack->xUnpacker, xReceivedBytes);
  return xReceivedBytes;
}

BaseType_t xMsgUnpackNext(MsgUnpackHandle_t xMsgUnpack) {
  return msgpack_unpacker_next(&xMsgUnpack->xUnpacker, &xMsgUnpack->xUnpacked);
}

/*!
 * \brief Type of last unpack.
 * \returns eMsgUnpackType for generic as-yet-unmapped type, else some more
 * specific type if currently known.
 */
BaseType_t xMsgUnpackType(MsgUnpackHandle_t xMsgUnpack) {
  switch (xMsgUnpack->xUnpacked.data.type) {
  case MSGPACK_OBJECT_STR:
    return eMsgUnpackTypeStr;
  default:
    return eMsgUnpackType;
  }
}

/*!
 * Converts standard \c bool to \c BaseType_t.
 */
BaseType_t xMsgUnpackBool(MsgUnpackHandle_t xMsgUnpack, BaseType_t *pxBool) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_BOOLEAN)
    return pdFAIL;
  if (pxBool)
    *pxBool = xMsgUnpack->xUnpacked.data.via.boolean;
  return pdPASS;
}

BaseType_t xMsgUnpackUInt32(MsgUnpackHandle_t xMsgUnpack, uint32_t *pxUInt32) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    return pdFAIL;
  if (pxUInt32)
    *pxUInt32 = xMsgUnpack->xUnpacked.data.via.u64;
  return pdPASS;
}

/*!
 * \brief Unpacks a signed 32-bit integer.
 *
 * Note that "negative" integer really means *signed* integer.
 */
BaseType_t xMsgUnpackInt32(MsgUnpackHandle_t xMsgUnpack, int32_t *pxInt32) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_NEGATIVE_INTEGER)
    return pdFAIL;
  /*
   * Does the following assignment reverse the sign extension?
   */
  if (pxInt32)
    *pxInt32 = xMsgUnpack->xUnpacked.data.via.i64;
  return pdPASS;
}

BaseType_t xMsgUnpackFloat32(MsgUnpackHandle_t xMsgUnpack, float *pxFloat32) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_FLOAT32)
    return pdFAIL;
  if (pxFloat32)
    *pxFloat32 = xMsgUnpack->xUnpacked.data.via.f64;
  return pdPASS;
}

/*!
 * \returns Succeeds only if the last unpack was a string object.
 */
BaseType_t xMsgUnpackStr(MsgUnpackHandle_t xMsgUnpack, const char **ppcStr,
                         size_t *pxStrLengthBytes) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_STR)
    return pdFAIL;
  if (ppcStr)
    *ppcStr = xMsgUnpack->xUnpacked.data.via.str.ptr;
  if (pxStrLengthBytes)
    *pxStrLengthBytes = xMsgUnpack->xUnpacked.data.via.str.size;
  return pdPASS;
}

/*!
 * Fails when the type mismatches.
 */
BaseType_t xMsgUnpackExtType(MsgUnpackHandle_t xMsgUnpack, const void **pvExt,
                             size_t *pxExtLengthBytes, int8_t cType) {
  if (xMsgUnpack->xUnpacked.data.type != MSGPACK_OBJECT_EXT ||
      xMsgUnpack->xUnpacked.data.via.ext.type != cType)
    return pdFAIL;
  if (pvExt)
    *pvExt = xMsgUnpack->xUnpacked.data.via.ext.ptr;
  if (pxExtLengthBytes)
    *pxExtLengthBytes = xMsgUnpack->xUnpacked.data.via.ext.size;
  return pdPASS;
}

/*!
 * \}
 */
