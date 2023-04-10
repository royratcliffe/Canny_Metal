/*
 * list_head.h
 */

#pragma once

#include "rwonce.h"

#include "containerof.h"

struct list_head {
  struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name)                                                                                           \
  { &(name), &(name) }

#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void init_list_head(struct list_head *list) {
  WRITE_ONCE(list->next, list);
  list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next) {
  next->prev = new;
  new->next = next;
  new->prev = prev;
  WRITE_ONCE(prev->next, new);
}

static inline void list_add(struct list_head *new, struct list_head *head) { __list_add(new, head, head->next); }

static inline void list_add_tail(struct list_head *new, struct list_head *head) { __list_add(new, head->prev, head); }

static inline void __list_del(struct list_head *prev, struct list_head *next) {
  next->prev = prev;
  WRITE_ONCE(prev->next, next);
}

/*!
 * Poisons the given list head by nullifying their next- and previous-pointers.
 */
static inline void list_del(struct list_head *head) {
  __list_del(head->prev, head->next);
  head->next = NULL;
  head->prev = NULL;
}

/*!
 * \brief Check for empty list.
 * \param head Head of list to check.
 */
static inline int list_is_empty(const struct list_head *head) { return READ_ONCE(head->next) == head; }

static inline int list_is_singular(const struct list_head *head) {
  return !list_is_empty(head) && head->next == head->prev;
}

#define LIST_ENTRY(head, type, member) CONTAINER_OF(head, type, member)

#define LIST_FIRST_ENTRY(head, type, member) LIST_ENTRY((head)->next, type, member)

#define LIST_LAST_ENTRY(head, type, member) LIST_ENTRY((head)->prev, type, member)

/*!
 * \brief Iterates over list, starting at head.
 * \param loop Pointer to `list_head` structure to use as loop cursor.
 */
#define LIST_FOR_EACH(loop, head) for (loop = (head)->next; loop != (head); loop = (loop)->next)
