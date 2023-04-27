// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_registered_can.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "registered_opaques.h"

static size_t prvHashOfOpaque(void *pvOpaque) {
  CAN_HandleTypeDef *pxCAN = pvOpaque;
  return (uint32_t)pxCAN->Instance >> 10U;
}

size_t xRegisteredCardinalOfCAN(CANHandle_t xCAN) {
  static void *pvOpaques[stm32xx_canMAX_INSTANCES];
  static struct RegisteredOpaques prvRegisteredOpaques = {
      .ppvOpaques = pvOpaques, .xNumberOfOpaques = stm32xx_canMAX_INSTANCES, .pxHashOfOpaqueFunction = prvHashOfOpaque};
  return xRegisteredCardinalOfOpaque(&prvRegisteredOpaques, xCAN);
}

#endif
