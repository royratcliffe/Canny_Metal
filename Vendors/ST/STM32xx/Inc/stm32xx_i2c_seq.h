/*
 * stm32xx_i2c_seq.h
 */

#pragma once

#include "stm32xx_i2c.h"

/*!
 * \brief I2C sequencer opaque handle.
 *
 * Use a forward structure declaration *without* definition hence opaque.
 *
 * Abstract STM32 I2C slave transfer.
 * The transfer has a direction, transmit or receive, representing the master's
 * first frame transfer bit. Transmit means master wants write. Receive means
 * master wants to read. The transfer also includes a matching address code.
 */
typedef struct I2CSeq *I2CSeqHandle_t;

/*!
 * \brief Creates a new dynamic I2C sequencer.
 */
I2CSeqHandle_t xI2CSeqCreate(I2CHandle_t xI2C);

/*!
 * \brief Deletes a previously-created I2C sequencer.
 */
void vI2CSeqDelete(I2CSeqHandle_t xI2CSeq);

/*!
 * \brief Sets up transfer direction, transmit or receive.
 *
 * Performs sanity validation. Only the least-significant bit determines the
 * direction: 0 for transmit, 1 for receive.
 */
void vI2CSeqTransferDirection(I2CSeqHandle_t xI2CSeq,
                              uint8_t ucTransferDirection);

void vI2CSeqTransmit(I2CSeqHandle_t xI2CSeq);

void vI2CSeqReceive(I2CSeqHandle_t xI2CSeq);

uint8_t ucI2CSeqTransferDirection(I2CSeqHandle_t xI2CSeq);

void vI2CSeqAddr(I2CSeqHandle_t xI2CSeq, uint8_t ucAddr);

uint8_t ucI2CSeqAddr(I2CSeqHandle_t xI2CSeq);

/*!
 * \brief Sets up a sequential master-mode interrupt-driven transfer.
 */
void vI2CSeqMasterIT(I2CSeqHandle_t xI2CSeq);

/*!
 * \brief Set up slave transfer for interrupt-driven usage.
 *
 * Notice the important transfer direction inversion for slaves. The slave
 * transmits during receive transfers.
 */
void vI2CSeqSlaveIT(I2CSeqHandle_t xI2CSeq);

/*!
 * \brief Allocates transfer buffer.
 * \param[in] xBufferLengthBytes Length of the buffer in bytes.
 */
void vI2CSeqBufferLengthBytes(I2CSeqHandle_t xI2CSeq,
                              size_t xBufferLengthBytes);

/*!
 * \brief Copies from data to buffer.
 */
void vI2CSeqCopyFrom(I2CSeqHandle_t xI2CSeq, const void *pvData);

/*!
 * \brief Copies transferred bytes only.
 */
void vI2CSeqCopyTo(I2CSeqHandle_t xI2CSeq, void *pvData);

/*!
 * Note that the buffer is mutable if allocated.
 */
void *pvI2CSeqBuffer(I2CSeqHandle_t xI2CSeq);

size_t xI2CSeqBufferLengthBytes(I2CSeqHandle_t xI2CSeq);

/*!
 * The I2C handle's transfer size is a down counter starting at the initial
 * transfer size.
 */
size_t xI2CSeqXferBytes(I2CSeqHandle_t xI2CSeq);

int xI2CSeqFrame(I2CSeqHandle_t xI2CSeq, uint32_t ulOptions);

/*!
 * \brief Transfers first or first-and-next frame.
 */
int xI2CSeqFirstFrame(I2CSeqHandle_t xI2CSeq);

int xI2CSeqNextFrame(I2CSeqHandle_t xI2CSeq);

int xI2CSeqLastFrame(I2CSeqHandle_t xI2CSeq);

int xI2CSeqNoOptionFrame(I2CSeqHandle_t xI2CSeq);

uint32_t xI2CSeqError(I2CSeqHandle_t xI2CSeq);
