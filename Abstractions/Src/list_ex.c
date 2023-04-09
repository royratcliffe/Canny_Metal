/*
 * list_ex.c
 */

#include "list_ex.h"

#include "task.h"

ListItem_t *pxListItemNew(void *pxOwner, TickType_t xValue) {
  ListItem_t *pxListItem = pvPortMalloc(sizeof(*pxListItem));
  if (pxListItem == NULL)
    return NULL;
  vListInitialiseItem(pxListItem);
  listSET_LIST_ITEM_OWNER(pxListItem, pxOwner);
  listSET_LIST_ITEM_VALUE(pxListItem, xValue);
  return pxListItem;
}

ListItem_t *pxListInsertNew(List_t *pxList, void *pxOwner, TickType_t xValue) {
  ListItem_t *pxListItem = pxListItemNew(pxOwner, xValue);
  configASSERT(pxListItem);
  vListInsert(pxList, pxListItem);
  return pxListItem;
}

ListItem_t *pxListInsertNewEnd(List_t *pxList, void *pxOwner,
                               TickType_t xValue) {
  ListItem_t *pxListItem = pxListItemNew(pxOwner, xValue);
  configASSERT(pxListItem);
  vListInsertEnd(pxList, pxListItem);
  return pxListItem;
}

const ListItem_t *pxListYield(List_t *const pxList,
                              BaseType_t (*pxYield)(void *pxOwner,
                                                    TickType_t xValue)) {
  const ListItem_t *pxListEnd = listGET_END_MARKER(pxList),
                   *pxListItem = listGET_HEAD_ENTRY(pxList);
  while (pxListItem != pxListEnd) {
    if (pxYield(listGET_LIST_ITEM_OWNER(pxListItem),
                listGET_LIST_ITEM_VALUE(pxListItem)) == pdPASS)
      return pxListItem;
    pxListItem = listGET_NEXT(pxListItem);
  }
  return NULL;
}

void vListYield(List_t *const pxList,
                BaseType_t (*pxYield)(void *pxOwner, TickType_t xValue)) {
  const ListItem_t *pxListEnd = listGET_END_MARKER(pxList),
                   *pxListItem = listGET_HEAD_ENTRY(pxList);
  while (pxListItem != pxListEnd) {
    if (pxYield(listGET_LIST_ITEM_OWNER(pxListItem),
                listGET_LIST_ITEM_VALUE(pxListItem)) == pdFAIL)
      return;
    pxListItem = listGET_NEXT(pxListItem);
  }
}
