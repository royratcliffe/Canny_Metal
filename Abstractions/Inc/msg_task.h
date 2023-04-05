/*
 * msg_task.h
 */

#pragma once

#ifndef msgEXT_TYPE_TASK
#define msgEXT_TYPE_TASK 'T'
#endif

#include "msg.h"

#include "task.h"

BaseType_t xMsgPackTask(MsgPackHandle_t xMsgPack, TaskHandle_t xTask);
BaseType_t xMsgUnpackTask(MsgUnpackHandle_t xMsgUnpack, TaskHandle_t *pxTask);
