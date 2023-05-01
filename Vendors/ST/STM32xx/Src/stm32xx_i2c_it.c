/*
 * stm32xx_i2c_it.c (Vendors, ST, STM32xx)
 */

#include "stm32xx_i2c_it.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "task_notify.h"

HAL_StatusTypeDef xI2CMasterTransmitIT(I2CHandle_t xI2C, uint8_t ucAddr, const void *pvData, size_t xDataLengthBytes,
                                       TickType_t xTicksToWait) {
  HAL_StatusTypeDef xStatus = HAL_I2C_Master_Transmit_IT(xI2C, ucAddr << 1U, (void *)pvData, xDataLengthBytes);
  if (xStatus != HAL_OK) return xStatus;
  return xI2CMasterWaitTransmitCplt(xTicksToWait);
}

HAL_StatusTypeDef xI2CMasterWaitTransmitCplt(TickType_t xTicksToWait) {
  return xTaskNotifyWaitExpected(stm32xxI2C_MASTER_TX_CPLT_NOTIFIED, xTicksToWait) ? HAL_OK : HAL_TIMEOUT;
}

void vI2CMasterNotifyTransmitCpltFromISR(TaskHandle_t xTask) {
  BaseType_t xWoken;
  xTaskNotifyFromISR(xTask, stm32xxI2C_MASTER_TX_CPLT_NOTIFIED, eSetBits, &xWoken);
  portYIELD_FROM_ISR(xWoken);
}

HAL_StatusTypeDef xI2CMasterReceiveIT(I2CHandle_t xI2C, uint8_t ucAddr, void *pvBuffer, size_t xBufferLengthBytes,
                                      TickType_t xTicksToWait) {
  HAL_StatusTypeDef xStatus = HAL_I2C_Master_Receive_IT(xI2C, ucAddr << 1U, pvBuffer, xBufferLengthBytes);
  if (xStatus != HAL_OK) return xStatus;
  return xI2CMasterWaitReceiveCplt(xTicksToWait);
}

HAL_StatusTypeDef xI2CMasterWaitReceiveCplt(TickType_t xTicksToWait) {
  return xTaskNotifyWaitExpected(stm32xxI2C_MASTER_RX_CPLT_NOTIFIED, xTicksToWait) ? HAL_OK : HAL_TIMEOUT;
}

void vI2CMasterNotifyReceiveCpltFromISR(TaskHandle_t xTask) {
  BaseType_t xWoken;
  xTaskNotifyFromISR(xTask, stm32xxI2C_MASTER_RX_CPLT_NOTIFIED, eSetBits, &xWoken);
  portYIELD_FROM_ISR(xWoken);
}

#endif
