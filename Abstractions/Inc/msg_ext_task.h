// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#ifndef msgEXT_TYPE_TASK
#define msgEXT_TYPE_TASK 'T'
#endif

#include "msg_binding.h"
#include "msg_unifier.h"

#include "task.h"

/*!
 * \brief Binds a task handle.
 *
 * Uses the \c msgEXT_TYPE_TASK extension to pack a task handle.
 */
BaseType_t xMsgBindTask(MsgBindingHandle_t xMsgBinding, TaskHandle_t xTask);

/*!
 * The task handle \e could be \c NULL. Unification passes for \c NULL handles.
 */
BaseType_t xMsgUnifyTask(MsgUnifierHandle_t xMsgUnifier, TaskHandle_t *pxTask);
