/*
 * stm32xx_i2c_slave_ti_tmp10x.h
 */

#pragma once

#include "stm32xx_i2c_slave.h"

#ifdef HAL_I2C_MODULE_ENABLED

#ifndef ti_tmp10xTASK_NAME
#define ti_tmp10xTASK_NAME "TMP10x"
#endif

#ifndef ti_tmp10xSTACK_DEPTH
#define ti_tmp10xSTACK_DEPTH 1024U
#endif

#ifndef ti_tmp10xPRIORITY
#define ti_tmp10xPRIORITY 32U
#endif

typedef struct TI_TMP10x *TI_TMP10xHandle_t;

TI_TMP10xHandle_t xTI_TMP10xCreate(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr);

BaseType_t xTI_TMP10xStart(TI_TMP10xHandle_t xTMP10x);

void vTI_TMP10xStop(TI_TMP10xHandle_t xTMP10x);

void vTI_TMP10xDelete(TI_TMP10xHandle_t xTMP10x);

#endif
