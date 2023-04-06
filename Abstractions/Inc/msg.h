/*
 * msg.h
 */

#pragma once

/*!
 * \file
 *
 * Do not forget to predefine \c MSGPACK_SBUFFER_INIT_SIZE for the packer.
 */

#include "FreeRTOS.h"
#include "message_buffer.h"

/*!
 * \defgroup MsgPack Message Packing
 * \{
 */

typedef struct MsgPack *MsgPackHandle_t;

MsgPackHandle_t xMsgPackNew();
void vMsgPackDestroy(MsgPackHandle_t xMsgPack);
void vMsgPackClear(MsgPackHandle_t xMsgPack);
BaseType_t xMsgPackBool(MsgPackHandle_t xMsgPack, BaseType_t xValue);
BaseType_t xMsgPackFloat(MsgPackHandle_t xMsgPack, float lValue);
BaseType_t xMsgPackStrWithBody(MsgPackHandle_t xMsgPack, const char *pcStr,
                               size_t xStrLengthBytes);
BaseType_t xMsgPackStr(MsgPackHandle_t xMsgPack, const char *pcStr);
BaseType_t xMsgPackExtWithBody(MsgPackHandle_t xMsgPack, const void *pvExt,
                               size_t xExtLengthBytes, int8_t cType);
size_t xMsgPackSendMessage(MsgPackHandle_t xMsgPack,
                           MessageBufferHandle_t xMessageBuffer,
                           TickType_t xTicksToWait);

/*!
 * \}
 */

/*!
 * \defgroup MsgUnpack Message Unpacking
 * \{
 */

#ifndef msgINITIAL_BUFFER_SIZE_BYTES
#define msgINITIAL_BUFFER_SIZE_BYTES 16U
#endif

enum msgUnpackNext {
  eMsgUnpackNextMemError = -2,
  eMsgUnpackNextParseError,
  eMsgUnpackNextContinue,
  eMsgUnpackNextExtraBytes,
  eMsgUnpackNextSuccess
};

typedef struct MsgUnpack *MsgUnpackHandle_t;

enum msgUnpackType { eMsgUnpackTypeStr, eMsgUnpackType };

MsgUnpackHandle_t xMsgUnpackCreate(size_t xInitialBufferSizeBytes);
MsgUnpackHandle_t xMsgUnpackNew();
void vMsgUnpackDestroy(MsgUnpackHandle_t xMsgUnpack);
size_t xMsgUnpackReceiveMessage(MsgUnpackHandle_t xMsgUnpack,
                                MessageBufferHandle_t xMessageBuffer,
                                TickType_t xTicksToWait);

BaseType_t xMsgUnpackNext(MsgUnpackHandle_t xMsgUnpack);

BaseType_t xMsgUnpackType(MsgUnpackHandle_t xMsgUnpack);
BaseType_t xMsgUnpackBool(MsgUnpackHandle_t xMsgUnpack, BaseType_t *pxBool);
BaseType_t xMsgUnpackUInt32(MsgUnpackHandle_t xMsgUnpack, uint32_t *pxUInt32);
BaseType_t xMsgUnpackInt32(MsgUnpackHandle_t xMsgUnpack, int32_t *pxInt32);
BaseType_t xMsgUnpackFloat32(MsgUnpackHandle_t xMsgUnpack, float *pxInt32);
BaseType_t xMsgUnpackStr(MsgUnpackHandle_t xMsgUnpack, const char **ppcStr,
                         size_t *pxStrLengthBytes);
BaseType_t xMsgUnpackExtType(MsgUnpackHandle_t xMsgUnpack, const void **pvExt,
                             size_t *pxExtLengthBytes, int8_t cType);

/*!
 * \}
 */
