// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#ifndef msgEXT_TYPE_OPAQUE
#  define msgEXT_TYPE_OPAQUE '*'
#endif

#include "msg_binding.h"
#include "msg_unifier.h"

BaseType_t xMsgBindOpaque(MsgBindingHandle_t xMsgBinding, void *pvOpaque);

BaseType_t xMsgUnifyOpaque(MsgUnifierHandle_t xMsgUnifier, void **ppvOpaque);
