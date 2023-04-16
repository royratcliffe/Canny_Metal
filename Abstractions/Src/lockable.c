/*
 * lockable.c
 */

#include "lockable.h"

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1) && (configUSE_RECURSIVE_MUTEXES == 1)

#include "semphr.h"
#include "task.h"

#include <memory.h>

struct Lockable {
  void *pvLockable;
  SemaphoreHandle_t xSemaphore;
#if configSUPPORT_STATIC_ALLOCATION == 1
  StaticSemaphore_t xStaticSemaphore;
#endif
};

LockableHandle_t xLockableCreate(void *pvLockable) {
  LockableHandle_t xLockable = pvPortMalloc(sizeof(*xLockable));
  configASSERT(xLockable);
  memset(xLockable, 0, sizeof(*xLockable));
  xLockable->pvLockable = pvLockable;
#if configSUPPORT_STATIC_ALLOCATION == 1
  xLockable->xSemaphore = xSemaphoreCreateRecursiveMutexStatic(&xLockable->xStaticSemaphore);
#else
  xLockable->xSemaphore = xSemaphoreCreateRecursiveMutex();
#endif
  return xLockable;
}

BaseType_t xLockableLock(LockableHandle_t xLockable, TickType_t xBlockTime) {
  return xSemaphoreTakeRecursive(xLockable->xSemaphore, xBlockTime);
}

void vLockableLock(LockableHandle_t xLockable) { (void)xLockableLock(xLockable, portMAX_DELAY); }

void *pvLockable(LockableHandle_t xLockable) { return xLockable->pvLockable; }

BaseType_t xLockableUnlock(LockableHandle_t xLockable) { return xSemaphoreGiveRecursive(xLockable->xSemaphore); }

void vLockableDelete(LockableHandle_t xLockable) {
  vSemaphoreDelete(xLockable->xSemaphore);
  vPortFree(xLockable);
}

#endif
