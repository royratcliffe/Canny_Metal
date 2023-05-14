/*
 * stm32xx_can_msgpack.h
 */

#pragma once

#include "stm32xx_can.h"

#include <msgpack.h>

/*!
 * \brief Packs a CAN receiver message.
 * \returns \c pdPASS if packing succeeds.
 *
 * Useful for interrupt-service routines that have no access to dynamic memory.
 */
BaseType_t xCANRxPack(const struct CANRx *pxCANRx, msgpack_packer *pxPacker);
