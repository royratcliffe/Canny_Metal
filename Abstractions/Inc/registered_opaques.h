/*
 * registered_opaques.h (Abstractions)
 */

/*!
 * \file
 *
 * The link to FreeRTOS is tenuous. Only the asserts utilise FreeRTOS \c
 * configASSERT for memory overruns. The naming conventions echo FreeRTOS
 * Hungarian naming conventions.
 */

#pragma once

#include <stddef.h>

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
