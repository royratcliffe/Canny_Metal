/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file registered_opaques.c
 * \brief Implementation of the RegisteredOpaques module.
 * \details This file implements the functions declared in registered_opaques.h
 * for the RegisteredOpaques module.
 */
#include <registered_opaques.h>

#include <errno.h>

/*!
 * \brief Searches for an opaque pointer.
 * \details The implementation design optimises for quickly finding a
 * previously-registered opaque. For example, it would be possible to collate
 * the \c NULL opaques while iterating.
 * \param xCardinal Starting cardinal typically based on the opaque's hash. Must
 * always be less than the number of opaque pointers.
 * \returns Pointer to opaque pointer.
 */
static void **ppvRegisteredOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque, size_t xCardinal);

/*!
 * \brief Computes the hash of an opaque pointer.
 * \details Uses the optional hash function to compute an optimised cardinal
 * offset for the opaque pointer. If no hash function is provided, returns zero
 * by default.
 * \param xRegisteredOpaques The registry of opaque pointers.
 * \param pvOpaque The opaque pointer to hash.
 * \returns Optimised cardinal offset of opaque pointer based on hash function,
 * or zero by default.
 */
static size_t xRegisteredHashOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

ptrdiff_t xRegisteredCardinalOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque) {
  size_t xCardinal = xRegisteredHashOfOpaque(xRegisteredOpaques, pvOpaque);
  void **ppvOpaque = ppvRegisteredOpaque(xRegisteredOpaques, pvOpaque, xCardinal);
  if (ppvOpaque == NULL) {
    ppvOpaque = ppvRegisteredOpaque(xRegisteredOpaques, NULL, xCardinal);
    if (ppvOpaque == NULL) {
      /*
       * The registry is full. This should never happen if the caller always
       * provides sufficient space for registered opaque pointers.
       */
      return -ENOMEM;
    }
    *ppvOpaque = pvOpaque;
  }
  return ppvOpaque - xRegisteredOpaques->ppvOpaques;
}

bool xOpaqueIsRegistered(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque) {
  return ppvRegisteredOpaque(xRegisteredOpaques, pvOpaque, xRegisteredHashOfOpaque(xRegisteredOpaques, pvOpaque)) !=
         NULL;
}

static void **ppvRegisteredOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque, size_t xCardinal) {
  for (size_t xOrdinal = xRegisteredOpaques->xNumberOfOpaques; xOrdinal; xOrdinal--) {
    void **ppvRegisteredOpaque = xRegisteredOpaques->ppvOpaques + xCardinal;
    if (*ppvRegisteredOpaque == pvOpaque) return ppvRegisteredOpaque;
    if (++xCardinal == xRegisteredOpaques->xNumberOfOpaques) xCardinal = 0U;
  }
  return NULL;
}

static size_t xRegisteredHashOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque) {
  if (xRegisteredOpaques->pxHashOfOpaqueFunction == NULL) return 0U;
  return xRegisteredOpaques->pxHashOfOpaqueFunction(pvOpaque) % xRegisteredOpaques->xNumberOfOpaques;
}
