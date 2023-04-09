/*
 * stm32xx_registered_i2c.c
 */

#include "stm32xx_registered_i2c.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "registered_opaques.h"

static size_t prvHashOfOpaque(void *pvOpaque) {
  I2C_HandleTypeDef *pxI2C = pvOpaque;
  return (uint32_t)pxI2C->Instance >> 10U;
}

size_t xRegisteredCardinalOfI2C(I2C_HandleTypeDef *pxI2C) {
  static void *pvOpaques[stm32xx_i2cMAX_INSTANCES];
  static struct RegisteredOpaques prvRegisteredOpaques = {
      .ppvOpaques = pvOpaques,
      .xNumberOfOpaques = stm32xx_i2cMAX_INSTANCES,
      .pxHashOfOpaqueFunction = prvHashOfOpaque};
  return xRegisteredCardinalOfOpaque(&prvRegisteredOpaques, pxI2C);
}

#endif
