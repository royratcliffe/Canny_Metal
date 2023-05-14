// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

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
   *
   * What happens if the unpacker's buffer capacity does not allow the message
   * buffer to drain. The mailbox becomes stuck with continuous receive failure.
   * Ensure that the unifier matches the message buffer size.
   */
  msgpack_unpacker_destroy(&xMsgUnifier->xUnpacker);
  msgpack_unpacker_init(&xMsgUnifier->xUnpacker, xMsgUnifier->xUnpacker.initial_buffer_size);
  size_t xReceivedBytes =
      xMessageBufferReceive(xMessageBuffer, msgpack_unpacker_buffer(&xMsgUnifier->xUnpacker),
                            msgpack_unpacker_buffer_capacity(&xMsgUnifier->xUnpacker), xTicksToWait);
  msgpack_unpacker_buffer_consumed(&xMsgUnifier->xUnpacker, xReceivedBytes);
  return xReceivedBytes;
}

/*
 * The implementation breaks unpacker encapsulation. The interface provides no
 * access to the used part of the buffer. Used therefore implies gone forever.
 * Resurrect the bits by accessing the unpacker members directly.
 */
size_t xMsgUnifierRelay(MsgUnifierHandle_t xMsgUnifier, MessageBufferHandle_t xMessageBuffer, TickType_t xTicksToWait) {
  return xMessageBufferSend(xMessageBuffer, xMsgUnifier->xUnpacker.buffer, xMsgUnifier->xUnpacker.used, xTicksToWait);
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
  case MSGPACK_OBJECT_ARRAY:
    return eMsgUnifyTypeArray;
  case MSGPACK_OBJECT_MAP:
    return eMsgUnifyTypeMap;
  case MSGPACK_OBJECT_BIN:
    return eMsgUnifyTypeBin;
  case MSGPACK_OBJECT_EXT:
    return eMsgUnifyTypeExt;
  default:
    return eMsgUnifyTypeOther;
  }
}

BaseType_t xMsgUnifyNil(MsgUnifierHandle_t xMsgUnifier) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_NIL) return pdFAIL;
  return pdPASS;
}

BaseType_t xMsgUnifyBool(MsgUnifierHandle_t xMsgUnifier, BaseType_t *pxValue) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_BOOLEAN) return pdFAIL;
  if (pxValue) *pxValue = xMsgUnifier->xUnpacked.data.via.boolean != false;
  return pdPASS;
}

BaseType_t xMsgUnifyUInt32(MsgUnifierHandle_t xMsgUnifier, uint32_t *pulValue) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_POSITIVE_INTEGER) return pdFAIL;
  if (pulValue) *pulValue = xMsgUnifier->xUnpacked.data.via.u64;
  return pdPASS;
}

BaseType_t xMsgUnifyInt32(MsgUnifierHandle_t xMsgUnifier, int32_t *plValue) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_NEGATIVE_INTEGER) return pdFAIL;
  /*
   * Does the following assignment reverse the sign extension?
   */
  if (plValue) *plValue = xMsgUnifier->xUnpacked.data.via.i64;
  return pdPASS;
}

BaseType_t xMsgUnifyFloat32(MsgUnifierHandle_t xMsgUnifier, float *plValue) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_FLOAT32) return pdFAIL;
  if (plValue) *plValue = xMsgUnifier->xUnpacked.data.via.f64;
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

BaseType_t xMsgUnifyStrCmp(MsgUnifierHandle_t xMsgUnifier, const char *pzStrCmp) {
  const char *pzStr;
  size_t xStrLengthBytes;
  if (!xMsgUnifyStr(xMsgUnifier, &pzStr, &xStrLengthBytes)) return pdFAIL;
  return strlen(pzStrCmp) == xStrLengthBytes && memcmp(pzStrCmp, pzStr, xStrLengthBytes) == 0;
}

BaseType_t xMsgUnifyArray(MsgUnifierHandle_t xMsgUnifier, size_t *pxNumberOfItems) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_ARRAY) return pdFAIL;
  if (pxNumberOfItems) *pxNumberOfItems = xMsgUnifier->xUnpacked.data.via.array.size;
  return pdPASS;
}

BaseType_t xMsgUnifyMap(MsgUnifierHandle_t xMsgUnifier, size_t *pxNumberOfItems) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_MAP) return pdFAIL;
  if (pxNumberOfItems) *pxNumberOfItems = xMsgUnifier->xUnpacked.data.via.map.size;
  return pdPASS;
}

BaseType_t xMsgUnifyMapStrKey(MsgUnifierHandle_t xMsgUnifier, const char *pzStrKey) {
  size_t xNumberOfItems;
  if (!xMsgUnifyMap(xMsgUnifier, &xNumberOfItems)) return pdFAIL;
  for (; xNumberOfItems != 0UL && xMsgUnify(xMsgUnifier); xNumberOfItems--) {
    BaseType_t xUnifyKey = xMsgUnifyStrCmp(xMsgUnifier, pzStrKey);
    BaseType_t xUnifyValue = xMsgUnify(xMsgUnifier);
    if (xUnifyKey) return xUnifyValue;
    if (!xUnifyValue) break;
  }
  return pdFAIL;
}

BaseType_t xMsgUnifyBin(MsgUnifierHandle_t xMsgUnifier, const void **pvBin, size_t *pxBinLengthBytes) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_BIN) return pdFAIL;
  if (pvBin) *pvBin = xMsgUnifier->xUnpacked.data.via.bin.ptr;
  if (pxBinLengthBytes) *pxBinLengthBytes = xMsgUnifier->xUnpacked.data.via.bin.size;
  return pdPASS;
}

BaseType_t xMsgUnifyExtType(MsgUnifierHandle_t xMsgUnifier, int8_t cType, const void **pvExt,
                            size_t *pxExtLengthBytes) {
  if (xMsgUnifier->xUnpacked.data.type != MSGPACK_OBJECT_EXT || xMsgUnifier->xUnpacked.data.via.ext.type != cType)
    return pdFAIL;
  if (pvExt) *pvExt = xMsgUnifier->xUnpacked.data.via.ext.ptr;
  if (pxExtLengthBytes) *pxExtLengthBytes = xMsgUnifier->xUnpacked.data.via.ext.size;
  return pdPASS;
}

/*!
 * \}
 */
