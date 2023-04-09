/*
 * stm32xx_i2c_xfer.h
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef void *I2CSeqXferHandle_t;

I2CSeqXferHandle_t xI2CSeqXferCreate();
void vI2CSeqXferDelete(I2CSeqXferHandle_t xI2CSeqXfer);
void vI2CSeqXferUseMasterIT(I2CSeqXferHandle_t xI2CSeqXfer, uint8_t ucAddr);
void vI2CSeqXferUseSlaveIT(I2CSeqXferHandle_t xI2CSeqXfer);
void vI2CSeqXferAllocBuffer(I2CSeqXferHandle_t xI2CSeqXfer, size_t xBufferLengthBytes);
void vI2CSeqXferCopyFrom(I2CSeqXferHandle_t xI2CSeqXfer, const void *pvData);
void vI2CSeqXferCopyTo(I2CSeqXferHandle_t xI2CSeqXfer, void *pvData);
void *pvI2CSeqXferBuffer(I2CSeqXferHandle_t xI2CSeqXfer);
size_t pvI2CSeqXferBufferLengthBytes(I2CSeqXferHandle_t xI2CSeqXfer);
int xI2CSeqXferFirstFrameTX(I2CSeqXferHandle_t xI2CSeqXfer);
int xI2CSeqXferNextFrameTX(I2CSeqXferHandle_t xI2CSeqXfer);
int xI2CSeqXferLastFrameTX(I2CSeqXferHandle_t xI2CSeqXfer);
