/* SPDX-License-Identifier: MIT */

/*!
 * \file cons_node.h
 * \brief Defines the structure and functions for cons nodes.
 * \details This header file defines the structure of a cons node---a
 * specialised data structure built on top of cons cells. Declares
 * functions for manipulating cons nodes, including accessing parent and
 * child nodes, and constructing a tree of cons nodes.
 */
#ifndef CONS_NODE_H
#define CONS_NODE_H

#include "cons.h"

/*!
 * \brief Constructs a cons node with the given cell and head.
 * \param cell The cons cell forming the basis of the node.
 * \param head The list of child nodes (sub-nodes).
 * \return A new cons node initialised with the specified cell and head.
 */
#define CONS_NODE(_cell_, _head_) ((struct cons_node){.cell = (_cell_), .head = (_head_)})

/*!
 * \brief A null cons node with no parent and no children.
 * \details This macro defines a null cons node, which is a node that
 * has its cell initialised to a null cons cell (with \c car set to \c
 * NULL and \c cdr set to \c CONS_NIL) and its head set to \c CONS_NIL,
 * indicating that it has no children. This can be used as a default or
 * placeholder node when constructing a tree of cons nodes.
 */
#define CONS_NODE_NULL CONS_NODE(CONS(NULL, CONS_NIL), CONS_NIL)

/*!
 * \brief Structure representing a cons node.
 * \details A cons node is a specialised data structure that builds on
 * top of cons cells. It contains a cons cell as its first member,
 * allowing it to be treated as a cons cell when needed. The \c head
 * field is a pointer to a list of child nodes (sub-nodes), which are
 * also cons nodes. This structure allows for the construction of a tree
 * of cons nodes, where each node can have a parent (super-node) and a
 * list of children (sub-nodes).
 */
struct cons_node {
  /*!
   * \brief The cons cell forming the basis of the node.
   * \details The \c car field of this cell points to the parent node
   * (super-node), while the \c cdr field links sibling nodes
   * (sub-nodes) together in a list.
   * \note The cell's cons structure is the first member of the node's
   * structure, so its address matches the address of the node. This
   * design allows for efficient access to the parent node and sibling
   * nodes while maintaining the flexibility of a cons cell.
   */
  struct cons cell;

  /*!
   * \brief Pointer to the list of child nodes (sub-nodes).
   * \details The head field is a pointer to a list of child nodes,
   * which are also cons nodes.
   */
  struct cons *head;
};

/*!
 * \brief Accessor for the parent node (super-node) of a cons node.
 * \param node The cons node to access.
 * \retval Pointer to the parent node, or \c NULL if there is no parent.
 * \details Retrieves the parent node from the \c car field of the cons cell.
 */
struct cons_node *cons_car_node(struct cons_node *node);

/*!
 * \brief Accessor for the next sibling node of a cons node.
 * \param node The cons node to access.
 * \return Pointer to the next sibling node, or \c NULL if there is no
 * next sibling node.
 * \note The next sibling node is stored in the \c cdr field of the
 * node's cell. Up-casts the \c cdr field of the cons cell to a pointer
 * to a node.
 */
struct cons_node *cons_cdr_node(struct cons_node *node);

/*!
 * \brief Accessor for the first child node (sub-node) of a cons node.
 * \param node The cons node to access.
 * \return Pointer to the first child node, or \c NULL if there are no
 * child nodes.
 * \note The first child node is stored in the \c head field of the
 * cons node. Up-casts the \c head field to a pointer to a node.
 */
struct cons_node *cons_sub_node(struct cons_node *node);

/*!
 * \brief Constructs a cons node with the specified sub-node and new super-node.
 * \param sub The sub-node to be linked.
 * \param super The new super-node to be linked.
 * \return Pointer to the super cons node for chaining.
 */
struct cons_node *cons_node(struct cons_node *sub, struct cons_node *super);

#endif /* CONS_NODE_H */
