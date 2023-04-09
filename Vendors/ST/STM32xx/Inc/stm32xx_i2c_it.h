/*
 * stm32xx_i2c_it.h (Vendors, ST, STM32xx)
 */

#pragma once

#include "FreeRTOS.h"
#include "task.h"

// for STM32xx hardware abstraction layer
#include "stm32xx_i2c.h"

/*!
 * \brief Master I2C transmit complete notification.
 */
#ifndef stm32xxI2C_MASTER_TX_CPLT_NOTIFIED
#define stm32xxI2C_MASTER_TX_CPLT_NOTIFIED (1UL << ('T' - 'A'))
#endif

/*!
 * \brief Master I2C receive complete notification.
 */
#ifndef stm32xxI2C_MASTER_RX_CPLT_NOTIFIED
#define stm32xxI2C_MASTER_RX_CPLT_NOTIFIED (1UL << ('R' - 'A'))
#endif

/*!
 * \brief Transmits over I2C as master using synchronised interrupts.
 *
 * \param[in] ucAddr I2C address \e without the shift.
 *
 * \returns Status of the hardware-abstraction layer transmit request.
 * Answers \c HAL_TIMEOUT if the notification fails to arrive before the
 * required timeout.
 *
 * Converts an asynchronous interrupt-driven master transmit to a
 * synchronous-mode transfer using task notification. Relies on an interrupt
 * service routine sending the task notification to the calling task.
 *
 * Casts the \c pvData to a mutable void pointer in order to work around the
 * hardware abstraction layer's \c const incorrect declaration. The layer does
 * \e not modify the data at \c pvData.
 */
HAL_StatusTypeDef xI2CMasterTransmitIT(I2CHandle_t xI2C, uint8_t ucAddr,
                                       const void *pvData,
                                       size_t xDataLengthBytes,
                                       TickType_t xTicksToWait);

/*!
 * \brief Notifies master transmit complete.
 *
 * Run the notification from the I2C interrupt service routine when the master
 * transmit operation completes.
 */
void vI2CMasterNotifyTransmitCpltFromISR(TaskHandle_t xTask);

HAL_StatusTypeDef xI2CMasterReceiveIT(I2CHandle_t xI2C, uint8_t ucAddr,
                                      void *pvBuffer, size_t xBufferLengthBytes,
                                      TickType_t xTicksToWait);

void vI2CMasterNotifyReceiveCpltFromISR(TaskHandle_t xTask);
