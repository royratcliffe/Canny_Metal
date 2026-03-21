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

struct RegisteredOpaques {
  void **ppvOpaques;
  size_t xNumberOfOpaques;
  size_t (*pxHashOfOpaqueFunction)(void *pvOpaque);
};

typedef struct RegisteredOpaques *RegisteredOpaques_t;

/*!
 * \brief Cardinal for opaque pointer.
 *
 * Registers the opaque pointer if not already registered. Uses the hash
 * function, if available, to place the opaque in a hash-optimised registry
 * location.
 *
 * Asserts if full. By design, always provide sufficient space for registered
 * opaque pointers. No function exists to unregister a pointer by design.
 */
size_t xRegisteredCardinalOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

/*!
 * \brief Non-mutating query of opaque by registry.
 *
 * \returns \c pdPASS if the opaque has already been registered.
 */
bool xOpaqueIsRegistered(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

#endif /* REGISTERED_OPAQUES_H */
