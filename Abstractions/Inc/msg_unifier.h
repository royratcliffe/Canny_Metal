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
#define msgunifierINITIAL_BUFFER_SIZE_BYTES 256U
#endif

enum msgUnify { eMsgUnifyMemError = -2, eMsgUnifyParseError, eMsgUnifyContinue, eMsgUnifyExtraBytes, eMsgUnifySuccess };

typedef struct MsgUnifier *MsgUnifierHandle_t;

enum msgUnpackType {
  eMsgUnifyTypeNil,
  eMsgUnifyTypeBool,
  eMsgUnifyTypeInt,
  eMsgUnifyTypeFloat,
  eMsgUnifyTypeStr,
  eMsgUnifyTypeArray,
  eMsgUnifyTypeMap,
  eMsgUnifyTypeBin,
  eMsgUnifyTypeExt,
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
 * \brief Relays a previously-received message.
 *
 * Relays the used buffer.
 */
size_t xMsgUnifierRelay(MsgUnifierHandle_t xMsgUnifier, MessageBufferHandle_t xMessageBuffer, TickType_t xTicksToWait);

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
 * \brief Unifies with nil.
 * \returns \c pdPASS if unification succeeds.
 */
BaseType_t xMsgUnifyNil(MsgUnifierHandle_t xMsgUnifier);

/*!
 * Converts standard \c bool to \c BaseType_t.
 */
BaseType_t xMsgUnifyBool(MsgUnifierHandle_t xMsgUnifier, BaseType_t *pxValue);

BaseType_t xMsgUnifyUInt32(MsgUnifierHandle_t xMsgUnifier, uint32_t *pulValue);

/*!
 * \brief Unpacks a signed 32-bit integer.
 *
 * Note that "negative" integer really means \e signed integer.
 */
BaseType_t xMsgUnifyInt32(MsgUnifierHandle_t xMsgUnifier, int32_t *plValue);

BaseType_t xMsgUnifyFloat32(MsgUnifierHandle_t xMsgUnifier, float *plValue);

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
 *
 * The function name might suggest an integer return value: a comparison integer
 * where negative means less than, positive more than, zero equal. That
 * information could be useful for optimising the message match, perhaps. The
 * return value must represent pass or fail in order to signal type and length
 * matching success, however.
 */
BaseType_t xMsgUnifyStrCmp(MsgUnifierHandle_t xMsgUnifier, const char *pcStrCmp);

BaseType_t xMsgUnifyArray(MsgUnifierHandle_t xMsgUnifier, size_t *pxNumberOfItems);

/*!
 * \brief Unifies with a map of items.
 * \param[out] pxNumberOfItems Pointer to received number of items on
 * success. Items are key-value pairs: key first, value second.
 */
BaseType_t xMsgUnifyMap(MsgUnifierHandle_t xMsgUnifier, size_t *pxNumberOfItems);

/*!
 * Requires an initial unification. Unifies the key by string comparison.
 * Unifies with the matched item's value on success. Either unifies with any
 * message material after the map on failure, or the end of the buffer in the
 * case of missing values or missing items.
 */
BaseType_t xMsgUnifyMapStrKey(MsgUnifierHandle_t xMsgUnifier, const char *pzStrKey);

BaseType_t xMsgUnifyBin(MsgUnifierHandle_t xMsgUnifier, const void **pvBin, size_t *pxBinLengthBytes);

/*!
 * \returns Fails when the type mismatches.
 */
BaseType_t xMsgUnifyExtType(MsgUnifierHandle_t xMsgUnifier, int8_t cType, const void **pvExt, size_t *pxExtLengthBytes);

/*!
 * \}
 */
