/*
 * stm32xx_i2c_slave_ti_tmp10x.h
 */

#pragma once

#include "stm32xx_i2c_slave.h"

#ifndef tmp10xTASK_NAME
#define tmp10xTASK_NAME "TMP10x"
#endif

#ifndef tmp10xSTACK_DEPTH
#define tmp10xSTACK_DEPTH 1024U
#endif

#ifndef tmp10xPRIORITY
#define tmp10xPRIORITY 32U
#endif

typedef struct TMP10x *TMP10xHandle_t;

TMP10xHandle_t xTMP10xCreate(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr);

BaseType_t xTMP10xStart(TMP10xHandle_t xTMP10x);

void vTMP10xStop(TMP10xHandle_t xTMP10x);

void vTMP10xDelete(TMP10xHandle_t xTMP10x);
