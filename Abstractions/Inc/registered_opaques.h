/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file registered_opaques.h
 * \brief Header file for the RegisteredOpaques module.
 * \details Provides a mechanism for registering opaque pointers and querying
 * their registration status. The RegisteredOpaques structure maintains a
 * registry of opaque pointers, allowing for efficient lookup and management of
 * these pointers. The functions provided by this module include registering an
 * opaque pointer and checking if an opaque pointer is already registered. The
 * module is designed to be flexible and extensible, allowing for the creation
 * of complex registries with various opaque pointers and behaviours.
 */
#ifndef REGISTERED_OPAQUES_H
#define REGISTERED_OPAQUES_H

#include <stddef.h>
#include <stdbool.h>

/*!
 * \brief The RegisteredOpaques structure.
 * \details This structure maintains a registry of opaque pointers, allowing for
 * efficient lookup and management of these pointers. It includes a pointer to
 * an array of opaque pointers, the number of opaque pointers, and an optional
 * hash function for optimising the placement of opaque pointers in the
 * registry.
 *
 * The registered opaque structure has no counter. The number of registered
 * opaque pointers corresponds to the total number of pointers less the number
 * of \c NULL pointers.
 */
struct RegisteredOpaques {
  void **ppvOpaques;       /*!< Array of opaque pointers. */
  size_t xNumberOfOpaques; /*!< Number of opaque pointers in the registry. */
  size_t (*pxHashOfOpaqueFunction)(
      void *pvOpaque); /*!< Optional hash function for optimising placement of opaque pointers. */
};

typedef struct RegisteredOpaques *RegisteredOpaques_t;

/*!
 * \brief Cardinal for opaque pointer.
 * \details Registers the opaque pointer if not already registered. Uses the hash
 * function, if available, to place the opaque in a hash-optimised registry
 * location.
 * \param xRegisteredOpaques The registry of opaque pointers to query.
 * \param pvOpaque The opaque pointer to register or query.
 * \retval The cardinal of the opaque pointer in the registry if it is already
 * registered, or the cardinal where it was registered if it was not already
 * registered.
 * \retval -ENOMEM if the registry is full and the opaque pointer could not be
 * registered.
 */
ptrdiff_t xRegisteredCardinalOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

/*!
 * \brief Non-mutating query of opaque by registry.
 * \param xRegisteredOpaques The registry of opaque pointers to query.
 * \param pvOpaque The opaque pointer to query.
 * \retval \c true if the opaque has already been registered.
 * \retval \c false if the opaque has not been registered.
 */
bool xOpaqueIsRegistered(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

#endif /* REGISTERED_OPAQUES_H */
