/*
 * stm32xx_can_unifier.h
 */

#pragma once

#include "stm32xx_can.h"

#include "msg_unifier.h"

/*!
 * \brief Unifies with a CAN receiver message.
 *
 * Mutates the given CAN receiver structure during unification, lazily. The
 * structure therefore partially mutates on failure.
 */
BaseType_t xMsgUnifyCANRx(MsgUnifierHandle_t xMsgUnifier, struct CANRx *pxCANRx);
