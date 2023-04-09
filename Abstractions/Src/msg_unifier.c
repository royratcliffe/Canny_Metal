/*
 * msg_unifier.c
 */

#include "msg_unifier.h"

#include <msgpack.h>

#include "task.h"

/*!
 * \addtogroup MsgUnifier
 * \{
 */

struct MsgUnifier {
  msgpack_unpacker xUnpacker;
  msgpack_unpacked xUnpacked;
};

MsgUnifierHandle_t xMsgUnifierCreate(size_t xInitialBufferSizeBytes) {
  MsgUnifierHandle_t xMsgUnpack = pvPortMalloc(sizeof(*xMsgUnpack));
  configASSERT(xMsgUnpack);
  msgpack_unpacker_init(&xMsgUnpack->xUnpacker, xInitialBufferSizeBytes);
  msgpack_unpacked_init(&xMsgUnpack->xUnpacked);
  return xMsgUnpack;
}

MsgUnifierHandle_t xMsgUnifierNew() { return xMsgUnifierCreate(msgunifierINITIAL_BUFFER_SIZE_BYTES); }

void vMsgUnifierDestroy(MsgUnifierHandle_t xMsgUnifier) {
  msgpack_unpacked_destroy(&xMsgUnifier->xUnpacked);
  msgpack_unpacker_destroy(&xMsgUnifier->xUnpacker);
}

BaseType_t xMsgUnifierReserve(MsgUnifierHandle_t xMsgUnifier, size_t xBufferSizeBytes) {
  return msgpack_unpacker_reserve_buffer(&xMsgUnifier->xUnpacker, xBufferSizeBytes);
}

size_t xMsgUnifierReceive(MsgUnifierHandle_t xMsgUnifier, MessageBufferHandle_t xMessageBuffer,
                          TickType_t xTicksToWait) {
  /*
   * Receiving automatically resets the packer. It does not reserve any
   * additional capacity automatically beyond that currently reserved.
   */
  msgpack_unpacker_reset(&xMsgUnifier->xUnpacker);
  size_t xReceivedBytes =
      xMessageBufferReceive(xMessageBuffer, msgpack_unpacker_buffer(&xMsgUnifier->xUnpacker),
                            msgpack_unpacker_buffer_capacity(&xMsgUnifier->xUnpacker), xTicksToWait);
  msgpack_unpacker_buffer_consumed(&xMsgUnifier->xUnpacker, xReceivedBytes);
  return xReceivedBytes;
}

BaseType_t xMsgUnify(MsgUnifierHandle_t xMsgUnifier) {
  return msgpack_unpacker_next(&xMsgUnifier->xUnpacker, &xMsgUnifier->xUnpacked);
}

BaseType_t xMsgUnifyType(MsgUnifierHandle_t xMsgUnifier) {
  switch (xMsgUnifier->xUnpacked.data.type) {
  case MSGPACK_OBJECT_NIL:
    return eMsgUnifyTypeNil;
  case MSGPACK_OBJECT_BOOLEAN:
    return eMsgUnifyTypeBool;
  case MSGPACK_OBJECT_POSITIVE_INTEGER:
  case MSGPACK_OBJECT_NEGATIVE_INTEGER:
    return eMsgUnifyTypeInt;
  case MSGPACK_OBJECT_FLOAT32:
  case MSGPACK_OBJECT_FLOAT64:
    return eMsgUnifyTypeFloat;
  case MSGPACK_OBJECT_STR:
    return eMsgUnifyTypeStr;
  default:
    return eMsgUnifyTypeOther;
  }
}

BaseType_t xMsgUnifyBool(MsgUnifierHandle_t xMsgUnifier, BaseType_t *pxBool) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_BOOLEAN) return pdFAIL;
  if (pxBool) *pxBool = xMsgUnifier->xUnpacked.data.via.boolean != false;
  return pdPASS;
}

BaseType_t xMsgUnifyUInt32(MsgUnifierHandle_t xMsgUnifier, uint32_t *pulUInt32) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_POSITIVE_INTEGER) return pdFAIL;
  if (pulUInt32) *pulUInt32 = xMsgUnifier->xUnpacked.data.via.u64;
  return pdPASS;
}

BaseType_t xMsgUnifyInt32(MsgUnifierHandle_t xMsgUnifier, int32_t *plInt32) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_NEGATIVE_INTEGER) return pdFAIL;
  /*
   * Does the following assignment reverse the sign extension?
   */
  if (plInt32) *plInt32 = xMsgUnifier->xUnpacked.data.via.i64;
  return pdPASS;
}

BaseType_t xMsgUnifyFloat32(MsgUnifierHandle_t xMsgUnifier, float *plFloat32) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_FLOAT32) return pdFAIL;
  if (plFloat32) *plFloat32 = xMsgUnifier->xUnpacked.data.via.f64;
  return pdPASS;
}

BaseType_t xMsgUnifyStr(MsgUnifierHandle_t xMsgUnifier, const char **ppcStr, size_t *pxStrLengthBytes) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_STR) return pdFAIL;
  if (ppcStr) *ppcStr = xMsgUnifier->xUnpacked.data.via.str.ptr;
  if (pxStrLengthBytes) *pxStrLengthBytes = xMsgUnifier->xUnpacked.data.via.str.size;
  return pdPASS;
}

BaseType_t xMsgUnifyStrDup(MsgUnifierHandle_t xMsgUnifier, char **ppcStrDup) {
  const char *pcStr;
  size_t xStrLengthBytes;
  if (!xMsgUnifyStr(xMsgUnifier, &pcStr, &xStrLengthBytes)) return pdFAIL;
  if (ppcStrDup) {
    char *pcStrDup = malloc(xStrLengthBytes + 1U);
    if (pcStrDup == NULL) return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
    (void)memcpy(pcStrDup, pcStr, xStrLengthBytes);
    pcStrDup[xStrLengthBytes] = '\0';
    *ppcStrDup = pcStrDup;
  }
  return pdPASS;
}

BaseType_t xMsgUnifyStrCmp(MsgUnifierHandle_t xMsgUnifier, const char *pcStrCmp) {
  const char *pcStr;
  size_t xStrLengthBytes;
  if (!xMsgUnifyStr(xMsgUnifier, &pcStr, &xStrLengthBytes)) return pdFAIL;
  return strlen(pcStrCmp) == xStrLengthBytes && memcmp(pcStrCmp, pcStr, xStrLengthBytes) == 0;
}

BaseType_t xMsgUnifyExtType(MsgUnifierHandle_t xMsgUnifier, const void **pvExt, size_t *pxExtLengthBytes,
                            int8_t cType) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_EXT || xMsgUnifier->xUnpacked.data.via.ext.type != cType)
    return pdFAIL;
  if (pvExt) *pvExt = xMsgUnifier->xUnpacked.data.via.ext.ptr;
  if (pxExtLengthBytes) *pxExtLengthBytes = xMsgUnifier->xUnpacked.data.via.ext.size;
  return pdPASS;
}

/*!
 * \}
 */