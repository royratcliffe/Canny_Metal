/*
 * stm32xx_i2c_seq.c
 */

#include "stm32xx_i2c_seq.h"

#include "FreeRTOS.h"
#include "task.h"

#include <memory.h>

/*!
 * \brief I2C sequencer for STM32.
 *
 * Wraps an abstract sequential I2C transfer. It encapsulates an I2C handle, a
 * dynamically-allocated buffer and virtualised transmit-receive methods. The
 * current version only supports dynamic buffering although future work could
 * add support for static buffers. Virtual transmit and receive support
 * interrupt-driven or DMA-driven transfers.
 *
 * The encapsulation provides persistent buffering using dynamic memory for
 * asynchronous transfers. It also performs slave transfer direction inversion.
 */
struct I2CSeq {
  I2CHandle_t xI2C;

  /*!
   * \brief Transmit and receive buffer for persisting transfer bytes.
   *
   * The underlying hardware abstraction layer fails to apply \c const
   * correctness to the transmit data pointer. It does not modify the transmit
   * bytes.
   */
  void *pvBuffer;

  size_t xBufferLengthBytes;
  HAL_StatusTypeDef (*pxTransfer[2U])(struct I2CSeq *pxI2CSeq, uint32_t ulOptions);
  HAL_StatusTypeDef (*pxNoOptionTransfer[2U])(struct I2CSeq *pxI2CSeq);
  uint8_t ucTransferDirection;

  /*!
   * \brief Master-mode I2C address without shift.
   *
   * Not used in slave mode except for introspection.
   */
  uint8_t ucAddr;
};

static HAL_StatusTypeDef prvMasterTransmitIT(struct I2CSeq *pxI2CSeq, uint32_t ulOptions) {
  return HAL_I2C_Master_Seq_Transmit_IT(pxI2CSeq->xI2C, pxI2CSeq->ucAddr << 1U, pxI2CSeq->pvBuffer,
                                        pxI2CSeq->xBufferLengthBytes, ulOptions);
}

static HAL_StatusTypeDef prvMasterReceiveIT(struct I2CSeq *pxI2CSeq, uint32_t ulOptions) {
  return HAL_I2C_Master_Seq_Receive_IT(pxI2CSeq->xI2C, pxI2CSeq->ucAddr << 1U, pxI2CSeq->pvBuffer,
                                       pxI2CSeq->xBufferLengthBytes, ulOptions);
}

static HAL_StatusTypeDef prvSlaveTransmitIT(struct I2CSeq *pxI2CSeq, uint32_t ulOptions) {
  return HAL_I2C_Slave_Seq_Transmit_IT(pxI2CSeq->xI2C, pxI2CSeq->pvBuffer, pxI2CSeq->xBufferLengthBytes, ulOptions);
}

static HAL_StatusTypeDef prvSlaveReceiveIT(struct I2CSeq *pxI2CSeq, uint32_t ulOptions) {
  return HAL_I2C_Slave_Seq_Receive_IT(pxI2CSeq->xI2C, pxI2CSeq->pvBuffer, pxI2CSeq->xBufferLengthBytes, ulOptions);
}

static HAL_StatusTypeDef prvMasterNoOptionTransmitIT(struct I2CSeq *pxI2CSeq) {
  return HAL_I2C_Master_Transmit_IT(pxI2CSeq->xI2C, pxI2CSeq->ucAddr << 1U, pxI2CSeq->pvBuffer,
                                    pxI2CSeq->xBufferLengthBytes);
}

static HAL_StatusTypeDef prvMasterNoOptionReceiveIT(struct I2CSeq *pxI2CSeq) {
  return HAL_I2C_Master_Receive_IT(pxI2CSeq->xI2C, pxI2CSeq->ucAddr << 1U, pxI2CSeq->pvBuffer,
                                   pxI2CSeq->xBufferLengthBytes);
}

static HAL_StatusTypeDef prvSlaveNoOptionTransmitIT(struct I2CSeq *pxI2CSeq) {
  return HAL_I2C_Slave_Transmit_IT(pxI2CSeq->xI2C, pxI2CSeq->pvBuffer, pxI2CSeq->xBufferLengthBytes);
}

static HAL_StatusTypeDef prvSlaveNoOptionReceiveIT(struct I2CSeq *pxI2CSeq) {
  return HAL_I2C_Slave_Receive_IT(pxI2CSeq->xI2C, pxI2CSeq->pvBuffer, pxI2CSeq->xBufferLengthBytes);
}

I2CSeqHandle_t xI2CSeqCreate(I2CHandle_t xI2C) {
  I2CSeqHandle_t xI2CSeq = pvPortMalloc(sizeof(*xI2CSeq));
  configASSERT(xI2CSeq);
  (void)memset(xI2CSeq, 0, sizeof(*xI2CSeq));
  xI2CSeq->xI2C = xI2C;
  return xI2CSeq;
}

void vI2CSeqDelete(I2CSeqHandle_t xI2CSeq) {
  if (xI2CSeq) {
    vPortFree(xI2CSeq->pvBuffer);
    vPortFree(xI2CSeq);
  }
}

void vI2CSeqTransferDirection(I2CSeqHandle_t xI2CSeq, uint8_t ucTransferDirection) {
  xI2CSeq->ucTransferDirection = ucTransferDirection & 0x01U;
}

void vI2CSeqTransmit(I2CSeqHandle_t xI2CSeq) { xI2CSeq->ucTransferDirection = I2C_DIRECTION_TRANSMIT; }

void vI2CSeqReceive(I2CSeqHandle_t xI2CSeq) { xI2CSeq->ucTransferDirection = I2C_DIRECTION_RECEIVE; }

uint8_t ucI2CSeqTransferDirection(I2CSeqHandle_t xI2CSeq) { return xI2CSeq->ucTransferDirection; }

void vI2CSeqAddr(I2CSeqHandle_t xI2CSeq, uint8_t ucAddr) { xI2CSeq->ucAddr = ucAddr & 0x7fU; }

uint8_t ucI2CSeqAddr(I2CSeqHandle_t xI2CSeq) { return xI2CSeq->ucAddr; }

void vI2CSeqMasterIT(I2CSeqHandle_t xI2CSeq) {
  xI2CSeq->pxTransfer[I2C_DIRECTION_TRANSMIT] = prvMasterTransmitIT;
  xI2CSeq->pxTransfer[I2C_DIRECTION_RECEIVE] = prvMasterReceiveIT;
  xI2CSeq->pxNoOptionTransfer[I2C_DIRECTION_TRANSMIT] = prvMasterNoOptionTransmitIT;
  xI2CSeq->pxNoOptionTransfer[I2C_DIRECTION_RECEIVE] = prvMasterNoOptionReceiveIT;
}

void vI2CSeqSlaveIT(I2CSeqHandle_t xI2CSeq) {
  xI2CSeq->pxTransfer[I2C_DIRECTION_TRANSMIT] = prvSlaveReceiveIT;
  xI2CSeq->pxTransfer[I2C_DIRECTION_RECEIVE] = prvSlaveTransmitIT;
  xI2CSeq->pxNoOptionTransfer[I2C_DIRECTION_TRANSMIT] = prvSlaveNoOptionReceiveIT;
  xI2CSeq->pxNoOptionTransfer[I2C_DIRECTION_RECEIVE] = prvSlaveNoOptionTransmitIT;
}

void vI2CSeqBufferLengthBytes(I2CSeqHandle_t xI2CSeq, size_t xBufferLengthBytes) {
  if (xI2CSeq->pvBuffer) {
    if (xI2CSeq->xBufferLengthBytes == xBufferLengthBytes) return;
    vPortFree(xI2CSeq->pvBuffer);
  }
  xI2CSeq->pvBuffer = pvPortMalloc(xI2CSeq->xBufferLengthBytes = xBufferLengthBytes);
  configASSERT(xI2CSeq->pvBuffer);
}

void vI2CSeqCopyFrom(I2CSeqHandle_t xI2CSeq, const void *pvData) {
  memcpy(xI2CSeq->pvBuffer, pvData, xI2CSeq->xBufferLengthBytes);
}

void vI2CSeqCopyTo(I2CSeqHandle_t xI2CSeq, void *pvData) {
  memcpy(pvData, xI2CSeq->pvBuffer, xI2CSeqXferBytes(xI2CSeq));
}

void *pvI2CSeqBuffer(I2CSeqHandle_t xI2CSeq) { return xI2CSeq->pvBuffer; }

size_t xI2CSeqBufferLengthBytes(I2CSeqHandle_t xI2CSeq) { return xI2CSeq->xBufferLengthBytes; }

size_t xI2CSeqXferBytes(I2CSeqHandle_t xI2CSeq) { return xI2CSeq->xBufferLengthBytes - xI2CSeq->xI2C->XferSize; }

int xI2CSeqFrame(I2CSeqHandle_t xI2CSeq, uint32_t ulOptions) {
  return xI2CSeq->pxTransfer[xI2CSeq->ucTransferDirection](xI2CSeq, ulOptions);
}

int xI2CSeqFirstFrame(I2CSeqHandle_t xI2CSeq) { return xI2CSeqFrame(xI2CSeq, I2C_FIRST_FRAME); }

int xI2CSeqNextFrame(I2CSeqHandle_t xI2CSeq) { return xI2CSeqFrame(xI2CSeq, I2C_NEXT_FRAME); }

int xI2CSeqLastFrame(I2CSeqHandle_t xI2CSeq) { return xI2CSeqFrame(xI2CSeq, I2C_LAST_FRAME); }

/*!
 * \brief Performs a no-option frame transfer.
 *
 * \code
 *   I2CSeqHandle_t xI2CSeq = xI2CSeqCreate(xI2C);
 *   vI2CSeqMasterIT(xI2CSeq);
 *   vI2CSeqTransmit(xI2CSeq);
 *   vI2CSeqAddr(xI2CSeq, ucAddr);
 *   vI2CSeqBufferLengthBytes(xI2CSeq, sizeof(ucData));
 *   vI2CSeqCopyFrom(xI2CSeq, ucData);
 *   HAL_StatusTypeDef xStatus = xI2CSeqNoOptionFrame(xI2CSeq);
 *   // Retain the sequencer until the transmit completes.
 *   // Important because the interrupt service routine accesses
 *   // its buffer during transmit.
 *   xStatus = xI2CMasterWaitTransmitCplt(portMAX_DELAY);
 *   vI2CSeqDelete(xI2CSeq);
 * \endcode
 */
int xI2CSeqNoOptionFrame(I2CSeqHandle_t xI2CSeq) {
  return xI2CSeq->pxNoOptionTransfer[xI2CSeq->ucTransferDirection](xI2CSeq);
}

uint32_t xI2CSeqError(I2CSeqHandle_t xI2CSeq) { return HAL_I2C_GetError(xI2CSeq->xI2C); }
