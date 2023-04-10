/*
 * hand_side.c
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sublicense, and/or sell  copies  of   the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "hand_side.h"

#include <malloc.h>
#include <string.h>

#if __USE_POSIX
#define stricmp(lhs, rhs) strcasecmp(lhs, rhs)
#endif

struct hand_side *push_hand_side(struct hand_side **top, void *left,
                                 void *right) {
  struct hand_side *push = malloc(sizeof(struct hand_side));
  if (push == NULL)
    return NULL;
  push->pop = *top;
  *top = push;
  push->left = left;
  push->right = right;
  return push;
}

void *pop_left_hand_side(struct hand_side **top, void *left) {
  for (struct hand_side *pop = *top; pop; pop = pop->pop) {
    if (pop->left == left) {
      void *result = pop->right;
      *top = pop->pop;
      free(pop);
      return result;
    }
    /*
     * Iterations beyond the first also move the top to the previous pop's next
     * pointer. A subsequent match will see a top at the previous pop's link
     * pointer, actually a sub-member of the pop's hand_side structure.
     */
    top = &pop->pop;
  }
  return NULL;
}

void free_hand_side(struct hand_side **top) {
  struct hand_side *pop = *top;
  if (pop == NULL)
    return;
  *top = NULL;
  while (pop) {
    void *pv = pop;
    pop = pop->pop;
    free(pv);
  }
}

void *for_left_and_right_hand_side(struct hand_side **top,
                                   void *(*and)(void *left, void *right,
                                                void *arg),
                                   void *arg) {
  for (struct hand_side *pop = *top; pop; pop = pop->pop) {
    void *result = (*and)(pop->left, pop->right, arg);
    if (result)
      return result;
  }
  return NULL;
}

void *vfor_left_and_right_hand_side(struct hand_side **top,
                                    void *(*and)(void *left, void *right,
                                                 va_list args),
                                    ...) {
  for (struct hand_side *pop = *top; pop; pop = pop->pop) {
    va_list args;
    va_start(args, and);
    void *result = (*and)(pop->left, pop->right, args);
    va_end(args);
    if (result)
      return result;
  }
  return NULL;
}

static void *left_eq(void *left, void *right, void *arg) {
  return left == arg ? right : NULL;
}

void *right_for_left_hand_side(struct hand_side **top, void *left) {
  return for_left_and_right_hand_side(top, left_eq, left);
}

static void *left_stricmp(void *left, void *right, void *arg) {
  return stricmp(left, arg) == 0 ? right : NULL;
}

void *right_for_left_stricmp_hand_side(struct hand_side **top,
                                       const char *left) {
  return for_left_and_right_hand_side(top, left_stricmp, (void *)left);
}

static void *right_eq(void *left, void *right, void *arg) {
  return right == arg ? left : NULL;
}

void *left_for_right_hand_side(struct hand_side **top, void *right) {
  return for_left_and_right_hand_side(top, right_eq, right);
}
