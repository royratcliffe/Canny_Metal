/*
 * list_ex.c
 */

#include "list_ex.h"

#include "task.h"

ListItem_t *pxListItemNew(void *pxOwner, TickType_t xValue) {
  ListItem_t *pxListItem = pvPortMalloc(sizeof(*pxListItem));
  if (pxListItem == NULL) return NULL;
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

ListItem_t *pxListInsertNewEnd(List_t *pxList, void *pxOwner, TickType_t xValue) {
  ListItem_t *pxListItem = pxListItemNew(pxOwner, xValue);
  configASSERT(pxListItem);
  vListInsertEnd(pxList, pxListItem);
  return pxListItem;
}

const ListItem_t *pxListYield(List_t *const pxList, BaseType_t (*pxYield)(void *pxOwner, TickType_t xValue)) {
  for (const ListItem_t *pxHeadEntry = listGET_HEAD_ENTRY(pxList), *pxEndMarker = listGET_END_MARKER(pxList),
                        *pxNextEntry;
       pxHeadEntry != pxEndMarker; pxHeadEntry = pxNextEntry) {
    pxNextEntry = listGET_NEXT(pxHeadEntry);
    if (pxYield(listGET_LIST_ITEM_OWNER(pxHeadEntry), listGET_LIST_ITEM_VALUE(pxHeadEntry)) == pdPASS)
      return pxHeadEntry;
  }
  return NULL;
}

void vListYield(List_t *const pxList, BaseType_t (*pxYield)(void *pxOwner, TickType_t xValue)) {
  for (const ListItem_t *pxHeadEntry = listGET_HEAD_ENTRY(pxList), *pxEndMarker = listGET_END_MARKER(pxList),
                        *pxNextEntry;
       pxHeadEntry != pxEndMarker; pxHeadEntry = pxNextEntry) {
    pxNextEntry = listGET_NEXT(pxHeadEntry);
    if (pxYield(listGET_LIST_ITEM_OWNER(pxHeadEntry), listGET_LIST_ITEM_VALUE(pxHeadEntry)) == pdFAIL) return;
  }
}
