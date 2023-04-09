/*
 * msg_unifier.h
 */

#pragma once

/*!
 * \file
 */

#include "FreeRTOS.h"
#include "message_buffer.h"

/*!
 * \defgroup MsgUnifier Message Unifier
 * \{
 */

#ifndef msgunifierINITIAL_BUFFER_SIZE_BYTES
#define msgunifierINITIAL_BUFFER_SIZE_BYTES 16U
#endif

enum msgUnify { eMsgUnifyMemError = -2, eMsgUnifyParseError, eMsgUnifyContinue, eMsgUnifyExtraBytes, eMsgUnifySuccess };

typedef struct MsgUnifier *MsgUnifierHandle_t;

enum msgUnpackType {
  eMsgUnifyTypeNil,
  eMsgUnifyTypeBool,
  eMsgUnifyTypeInt,
  eMsgUnifyTypeFloat,
  eMsgUnifyTypeStr,
  eMsgUnifyTypeOther
};

MsgUnifierHandle_t xMsgUnifierCreate(size_t xInitialBufferSizeBytes);

MsgUnifierHandle_t xMsgUnifierNew();

void vMsgUnifierDestroy(MsgUnifierHandle_t xMsgUnifier);

/*!
 * \brief Reserves additional buffer capacity for the unifier.
 *
 * Attempts to expand the unification buffer if the current amount of free space
 * is less than the given number of buffer bytes.
 */
BaseType_t xMsgUnifierReserve(MsgUnifierHandle_t xMsgUnifier, size_t xBufferSizeBytes);

/*!
 * \returns Answers zero if the unpacker fails to reserve sufficient memory to
 * hold the incoming message.
 *
 * Receives zero or one message from the given message buffer.
 *
 * Unpacks directly to the unpacking buffer. Begins by reserving the necessary
 * unpack buffer space.
 */
size_t xMsgUnifierReceive(MsgUnifierHandle_t xMsgUnifier, MessageBufferHandle_t xMessageBuffer,
                          TickType_t xTicksToWait);

/*!
 * \brief Unifies with the \e next message object.
 */
BaseType_t xMsgUnify(MsgUnifierHandle_t xMsgUnifier);

/*!
 * \brief Type of last unify.
 * \returns eMsgUnifierType for generic as-yet-unmapped type, else some more
 * specific type if currently known.
 */
BaseType_t xMsgUnifyType(MsgUnifierHandle_t xMsgUnifier);

/*!
 * Converts standard \c bool to \c BaseType_t.
 */
BaseType_t xMsgUnifyBool(MsgUnifierHandle_t xMsgUnifier, BaseType_t *pxBool);

BaseType_t xMsgUnifyUInt32(MsgUnifierHandle_t xMsgUnifier, uint32_t *pulUInt32);

/*!
 * \brief Unpacks a signed 32-bit integer.
 *
 * Note that "negative" integer really means \e signed integer.
 */
BaseType_t xMsgUnifyInt32(MsgUnifierHandle_t xMsgUnifier, int32_t *plInt32);

BaseType_t xMsgUnifyFloat32(MsgUnifierHandle_t xMsgUnifier, float *plFloat32);

/*!
 * \returns Succeeds only if the last unpack was a string object.
 */
BaseType_t xMsgUnifyStr(MsgUnifierHandle_t xMsgUnifier, const char **ppcStr, size_t *pxStrLengthBytes);

/*!
 * \param[out] ppcStr Pointer to string pointer. The type is omits \c const to
 * allow for freeing of the duplicate copy.
 *
 * \returns \c pdPASS if unification \e and allocation succeeds. Fails either if
 * cannot unify returning \c pdFAIL \e or allocation fails returning \c
 * errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY.
 *
 * Unifies with a string by allocating and copying the string to a
 * null-terminated dynamic buffer. Free the allocated buffer after usage.
 */
BaseType_t xMsgUnifyStrDup(MsgUnifierHandle_t xMsgUnifier, char **ppcStrDup);

/*!
 * \brief Unifies with precisely-matching null-terminated string.
 *
 * Performance is good: comparison proceeds by type first, followed by length
 * and finally by body character-by-character with early-out optimisation.
 */
BaseType_t xMsgUnifyStrCmp(MsgUnifierHandle_t xMsgUnifier, const char *pcStrCmp);

/*!
 * \returns Fails when the type mismatches.
 */
BaseType_t xMsgUnifyExtType(MsgUnifierHandle_t xMsgUnifier, const void **pvExt, size_t *pxExtLengthBytes, int8_t cType);

/*!
 * \}
 */