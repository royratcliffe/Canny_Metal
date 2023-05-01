/*
 * registered_opaques.c
 *
 * The registered opaque structure has no counter. The number of registered
 * opaque pointers corresponds to the total number of pointers less the number
 * of \c NULL pointers.
 */

#include "registered_opaques.h"

/*
 * Only for \c configASSERT macro.
 */
#include "task.h"

static void **ppvRegisteredOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque, size_t xCardinal);

static size_t xRegisteredHashOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque);

size_t xRegisteredCardinalOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque) {
  size_t xCardinal = xRegisteredHashOfOpaque(xRegisteredOpaques, pvOpaque);
  void **ppvOpaque = ppvRegisteredOpaque(xRegisteredOpaques, pvOpaque, xCardinal);
  if (ppvOpaque == NULL) {
    ppvOpaque = ppvRegisteredOpaque(xRegisteredOpaques, NULL, xCardinal);
    configASSERT(ppvOpaque);
    *ppvOpaque = pvOpaque;
  }
  return ppvOpaque - xRegisteredOpaques->ppvOpaques;
}

BaseType_t xOpaqueIsRegistered(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque)
{
  return ppvRegisteredOpaque(xRegisteredOpaques, pvOpaque, xRegisteredHashOfOpaque(xRegisteredOpaques, pvOpaque)) != NULL;
}

/*!
 * \brief Searches for an opaque pointer.
 *
 * \param xCardinal Starting cardinal typically based on the opaque's hash. Must
 * always be less than the number of opaque pointers.
 *
 * \returns Pointer to opaque pointer.
 *
 * The implementation design optimises for quickly finding a
 * previously-registered opaque. For example, it would be possible to collate
 * the \c NULL opaques while iterating.
 */
static void **ppvRegisteredOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque, size_t xCardinal) {
  for (size_t xOrdinal = xRegisteredOpaques->xNumberOfOpaques; xOrdinal; xOrdinal--) {
    void **ppvRegisteredOpaque = xRegisteredOpaques->ppvOpaques + xCardinal;
    if (*ppvRegisteredOpaque == pvOpaque) return ppvRegisteredOpaque;
    if (++xCardinal == xRegisteredOpaques->xNumberOfOpaques) xCardinal = 0U;
  }
  return NULL;
}

/*!
 * \brief Computes the hash of an opaque pointer.
 * \returns Optimised cardinal offset of opaque pointer based on hash function,
 * or zero by default.
 */
static size_t xRegisteredHashOfOpaque(RegisteredOpaques_t xRegisteredOpaques, void *pvOpaque) {
  if (xRegisteredOpaques->pxHashOfOpaqueFunction == NULL) return 0U;
  return xRegisteredOpaques->pxHashOfOpaqueFunction(pvOpaque) % xRegisteredOpaques->xNumberOfOpaques;
}
