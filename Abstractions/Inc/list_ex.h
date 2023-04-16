/*
 * list_ex.h
 */

#pragma once

// FreeRTOS
#include "FreeRTOS.h"
#include "list.h"

/*!
 * \brief Constructs new list item.
 *
 * \returns New dynamically-allocated list item or \c NULL if out of memory.
 *
 * \note Be aware the item value \c xValue typically spans 32-bits but _could_
 * have 16 bits on some smaller systems. If the FreeRTOS configuration defines
 * \c configUSE_16_BIT_TICKS as 1, the tick type is an unsigned 16-bit integer
 * type.
 */
ListItem_t *pxListItemNew(void *pxOwner, TickType_t xValue);

/*!
 * \brief Inserts new list item.
 *
 * \note Asserts successful allocation.
 */
ListItem_t *pxListInsertNew(List_t *pxList, void *pxOwner, TickType_t xValue);

/*!
 * \brief Inserts item at end of list.
 *
 * Inserts a new item at the very end of the list _without_ sorting by item
 * value. Thus, the \c xValue parameter is non-functional with respect to the
 * container albeit appearing in list yields.
 */
ListItem_t *pxListInsertNewEnd(List_t *pxList, void *pxOwner, TickType_t xValue);

/*!
 * \brief Yields list item owners and item values as pairs while yielding fails.
 *
 * \param pxYield Pointer to yield function taking the owner-value tuple for
 * each list item until the yield answers \c pdPASS.
 *
 * \returns Answers the passing list item, else \c NULL if no matching list
 * item. The \c pxYield function determines the match, success or failure, pass
 * or fail.
 *
 * The return value is \c const by design for compatibility; if not \c NULL, the
 * result is a immutable list item. Such items reference their container list
 * and even if immutable can detach using \c uxListRemove.
 *
 * The list item iteration steps ahead before calling the yield function. The
 * iteration continues normally even if the yield function removes the item,
 * although the arguments only pass the item's owner and value members; it does
 * not pass the item itself.
 */
const ListItem_t *pxListYield(List_t *const pxList, BaseType_t (*pxYield)(void *pxOwner, TickType_t xValue));

/*!
 * \brief Yields owners and their values.
 *
 * \param pxYield Pointer to yield function that receives the owner and item
 * value. The yield continues for each item while the function returns \c
 * pdPASS.
 *
 * \returns Answers the passing item.
 */
void vListYield(List_t *const pxList, BaseType_t (*pxYield)(void *pxOwner, TickType_t xValue));
