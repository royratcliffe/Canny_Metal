/*
 * stm32xx_i2c_xfer.c
 */

#include "stm32xx_i2c_xfer.h"
#include "stm32xx_i2c.h"

#include "FreeRTOS.h"
#include "task.h"

#include <memory.h>

/*!
 * Wraps an abstract sequential I2C transfer. It encapsulates an I2C handle, a
 * dynamically-allocated buffer and virtualised transmit-receive methods. The
 * current version only supports dynamic buffering although future work could
 * add support for static buffers. Virtual transmit and receive support
 * interrupt-driven or DMA-driven transfers.
 */
struct I2CSeqXfer {
  I2CHandle_t xI2C;
  void *pvBuffer;
  size_t xBufferLengthBytes;
  HAL_StatusTypeDef (*pxTransmit)(struct I2CSeqXfer *pxI2CSeqXfer,
                                  uint32_t ulOptions);
  HAL_StatusTypeDef (*pxReceive)(struct I2CSeqXfer *pxI2CSeqXfer,
                                 uint32_t ulOptions);
  uint8_t ucAddr;
};

HAL_StatusTypeDef prvMasterTransmitIT(struct I2CSeqXfer *pxI2CSeqXfer,
                                      uint32_t ulOptions) {
  return HAL_I2C_Master_Seq_Transmit_IT(
      pxI2CSeqXfer->xI2C, pxI2CSeqXfer->ucAddr << 1U, pxI2CSeqXfer->pvBuffer,
      pxI2CSeqXfer->xBufferLengthBytes, ulOptions);
}

HAL_StatusTypeDef prvMasterReceiveIT(struct I2CSeqXfer *pxI2CSeqXfer,
                                     uint32_t ulOptions) {
  return HAL_I2C_Master_Seq_Receive_IT(
      pxI2CSeqXfer->xI2C, pxI2CSeqXfer->ucAddr << 1U, pxI2CSeqXfer->pvBuffer,
      pxI2CSeqXfer->xBufferLengthBytes, ulOptions);
}

HAL_StatusTypeDef prvSlaveTransmitIT(struct I2CSeqXfer *pxI2CSeqXfer,
                                     uint32_t ulOptions) {
  return HAL_I2C_Slave_Seq_Transmit_IT(
      pxI2CSeqXfer->xI2C, pxI2CSeqXfer->pvBuffer,
      pxI2CSeqXfer->xBufferLengthBytes, ulOptions);
}

HAL_StatusTypeDef prvSlaveReceiveIT(struct I2CSeqXfer *pxI2CSeqXfer,
                                    uint32_t ulOptions) {
  return HAL_I2C_Slave_Seq_Receive_IT(
      pxI2CSeqXfer->xI2C, pxI2CSeqXfer->pvBuffer,
      pxI2CSeqXfer->xBufferLengthBytes, ulOptions);
}

I2CSeqXferHandle_t xI2CSeqXferCreate() {
  struct I2CSeqXfer *pxI2CSeqXfer = pvPortMalloc(sizeof(*pxI2CSeqXfer));
  configASSERT(pxI2CSeqXfer);
  return pxI2CSeqXfer;
}

void vI2CSeqXferDelete(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  if (pxI2CSeqXfer) {
    vPortFree(pxI2CSeqXfer->pvBuffer);
    vPortFree(pxI2CSeqXfer);
  }
}

void vI2CSeqXferUseMasterIT(I2CSeqXferHandle_t xI2CSeqXfer, uint8_t ucAddr) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  pxI2CSeqXfer->pxTransmit = prvMasterTransmitIT;
  pxI2CSeqXfer->pxReceive = prvMasterReceiveIT;
  pxI2CSeqXfer->ucAddr = ucAddr;
}

/*!
 * \brief Set up transfer for interrupt-driven usage.
 */
void vI2CSeqXferUseSlaveIT(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  pxI2CSeqXfer->pxTransmit = prvSlaveTransmitIT;
  pxI2CSeqXfer->pxReceive = prvSlaveReceiveIT;
}

/*!
 * \brief Allocates transfer buffer.
 * \param[in] xBufferLengthBytes Length of the buffer in bytes.
 */
void vI2CSeqXferAllocBuffer(I2CSeqXferHandle_t xI2CSeqXfer,
                            size_t xBufferLengthBytes) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  if (pxI2CSeqXfer->pvBuffer) {
    if (pxI2CSeqXfer->xBufferLengthBytes == xBufferLengthBytes)
      return;
    vPortFree(pxI2CSeqXfer->pvBuffer);
  }
  pxI2CSeqXfer->pvBuffer =
      pvPortMalloc(pxI2CSeqXfer->xBufferLengthBytes = xBufferLengthBytes);
  configASSERT(pxI2CSeqXfer->pvBuffer);
}

/*!
 * \brief Copies from data to buffer.
 */
void vI2CSeqXferCopyFrom(I2CSeqXferHandle_t xI2CSeqXfer, const void *pvData) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  memcpy(pxI2CSeqXfer->pvBuffer, pvData, pxI2CSeqXfer->xBufferLengthBytes);
}

void vI2CSeqXferCopyTo(I2CSeqXferHandle_t xI2CSeqXfer, void *pvData) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  memcpy(pvData, pxI2CSeqXfer->pvBuffer, pxI2CSeqXfer->xBufferLengthBytes);
}

void *pvI2CSeqXferBuffer(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pvBuffer;
}

size_t pvI2CSeqXferBufferLengthBytes(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->xBufferLengthBytes;
}

int xI2CSeqXferFirstFrameTX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxTransmit(pxI2CSeqXfer, I2C_FIRST_FRAME);
}

int xI2CSeqXferNextFrameTX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxTransmit(pxI2CSeqXfer, I2C_NEXT_FRAME);
}

int xI2CSeqXferLastFrameTX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxTransmit(pxI2CSeqXfer, I2C_LAST_FRAME);
}

int xI2CSeqXferFirstFrameRX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxReceive(pxI2CSeqXfer, I2C_FIRST_FRAME);
}

int xI2CSeqXferNextFrameRX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxReceive(pxI2CSeqXfer, I2C_NEXT_FRAME);
}

int xI2CSeqXferLastFrameRX(I2CSeqXferHandle_t xI2CSeqXfer) {
  struct I2CSeqXfer *pxI2CSeqXfer = xI2CSeqXfer;
  return pxI2CSeqXfer->pxReceive(pxI2CSeqXfer, I2C_LAST_FRAME);
}
