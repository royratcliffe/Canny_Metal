/* SPDX-License-Identifier: MIT */

/*!
 * \file cons_node.c
 * \brief Implements functions for manipulating cons nodes.
 * \details This file implements functions for working with cons nodes, which
 * are a specialised data structure built on top of cons cells. The functions
 * allow for constructing a tree of cons nodes, where each node can have a
 * parent (super-node) and a list of children (sub-nodes). The operations include
 * linking and unlinking nodes in the tree structure, as well as accessing the
 * parent and child nodes.
 */
#include <cons_node.h>

/*
 * This definition exists as a token. It does nothing in practice
 * because a node's cell (currently) lives at the start of the node's
 * structure. Subtracting a zero offset (the offset of a cell from its
 * node) does nothing. The compiler does all the work. Nothing happens
 * at runtime. This exists to preserve container correctness and to
 * avoid undefined behaviour.
 */
#ifdef containerof
#define NODE_FROM_CELL(_cell_) containerof(_cell_, struct cons_node, cell)
#else
#define NODE_FROM_CELL(_cell_) ((struct cons_node *)((char *)(_cell_) - offsetof(struct cons_node, cell)))
#endif

/*
 * The following ternary operator is not strictly necessary, but better
 * to check for the NULL pointer before doing the container adjustment,
 * as it avoids unnecessary pointer arithmetic when the input is NULL.
 */
static inline struct cons_node *node_from_cell(struct cons *cell) { return CONS_NOT_NIL_P(cell) ? NODE_FROM_CELL(cell) : NULL; }

struct cons_node *cons_car_node(struct cons_node *node) { return (struct cons_node *)cons_car(&node->cell); }

struct cons_node *cons_cdr_node(struct cons_node *node) { return node_from_cell(cons_cdr(&node->cell)); }

struct cons_node *cons_sub_node(struct cons_node *node) { return node_from_cell(node->head); }

/*
 * Constructs a tree of nodes.
 */
struct cons_node *cons_node(struct cons_node *sub, struct cons_node *super) {
  struct cons_node *car = cons_car_node(sub);
  if (car != NULL) {
    /*
     * The sub-node already has a super-node. Remove the sub-node from
     * the list of sub-nodes of the current super-node. Remove it even
     * if the current super-node is the same as the new super-node,
     * because the sub-node may be in a different position in the list
     * of sub-nodes of the new super-node when re-constructing the same
     * sub-super linkage.
     */
    (void)cons_remove(&car->head, &sub->cell);
    cons_rplaca(&sub->cell, NULL);
  }
  if (super != NULL) {
    /*
     * The new super-node is not NULL. Prepend the sub-node to the list
     * of sub-nodes of the new super-node. Set the car field of the
     * sub-node to point to the new super-node.
     *
     * This could be redundant if the sub-node is already a sub-node of
     * the new super-node, but it is simpler to just do it
     * unconditionally than to check for that case.
     */
    (void)cons(&super->head, &sub->cell);
    cons_rplaca(&sub->cell, super);
  }
  return super;
}
