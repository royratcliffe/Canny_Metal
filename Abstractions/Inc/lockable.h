/*
 * lockable.c
 */

#pragma once

#include "FreeRTOS.h"

typedef struct Lockable *LockableHandle_t;

/*!
 * \brief Creates a new lockable.
 * \param[in] pvLockable Pointer to lockable resource.
 *
 * Binds a recursive mutex to the lockable resource. By contract,
 * the lockable accessing code agrees to lock \e before access and unlock after access.
 */
LockableHandle_t xLockableCreate(void *pvLockable);

BaseType_t xLockableLock(LockableHandle_t xLockable, TickType_t xBlockTime);

void vLockableLock(LockableHandle_t xLockable);

/*!
 * \brief Accesses the lockable resource.
 *
 * Do \e not access the lockable \e without first locking.
 */
void *pvLockable(LockableHandle_t xLockable);

BaseType_t xLockableUnlock(LockableHandle_t xLockable);

void vLockableDelete(LockableHandle_t xLockable);
