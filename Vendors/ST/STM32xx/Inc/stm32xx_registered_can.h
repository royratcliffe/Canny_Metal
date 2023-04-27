// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include "stm32xx_can.h"

#ifdef HAL_CAN_MODULE_ENABLED

size_t xRegisteredCardinalOfCAN(CANHandle_t xCAN);

#endif
