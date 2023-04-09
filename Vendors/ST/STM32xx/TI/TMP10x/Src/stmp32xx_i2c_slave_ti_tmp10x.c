/*
 * stm32xx_i2c_slave_ti_tmp10x.c
 */

#include "stm32xx_i2c_slave_ti_tmp10x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32xx_i2c_seq.h"
#include "stm32xx_i2c_slave.h"

#include <memory.h>
#include <stdio.h>

#define tmp10xSTOP_NOTIFIED (1UL << ('Z' - 'A'))

struct TMP10x {
  TaskHandle_t xTask;
  I2CSlaveHandle_t xI2CSlave;
  uint8_t ucAddr;
};

/*!
 * \brief Set up device at address.
 *
 * For the sake of simplicity, all errors assert. Assume the happy path.
 */
static portTASK_FUNCTION(prvTMP10xTask, pvParameters) {
  struct TMP10x *xTMP10x = pvParameters;

  /*
   * 16-bit register file.
   */
  uint16_t usFile[] = {0x1234U, 0xff00U, 0x1111U, 0xffffU};

  void prvAddr(I2CSeqHandle_t xI2CSeq) {
    switch (ucI2CSeqTransferDirection(xI2CSeq)) {
      HAL_StatusTypeDef xStatus;
    case I2C_DIRECTION_TRANSMIT:
      /*
       * Receive the first and next frames.
       */
      vI2CSeqBufferLengthBytes(xI2CSeq, 3U);
      xStatus = xI2CSeqFirstFrame(xI2CSeq);
      configASSERT(xStatus == HAL_OK);
      break;
    case I2C_DIRECTION_RECEIVE:
      /*
       * Transmit the last frames.
       */
      switch (xI2CSeqXferBytes(xI2CSeq)) {
        uint8_t ucPtr;
        uint16_t usReg;
        uint8_t ucBig[2];
      case 1U:
        vI2CSeqCopyTo(xI2CSeq, &ucPtr);
        /*
         * Send big-endian word.
         */
        usReg = usFile[ucPtr & 0x03U];
        ucBig[0] = usReg >> 8U;
        ucBig[1] = usReg;
        vI2CSeqCopyFrom(xI2CSeq, ucBig);
        xStatus = xI2CSeqLastFrame(xI2CSeq);
        configASSERT(xStatus == HAL_OK);
      }
    }
  }

  /*
   * The slave buffer receive completes.
   */
  void prvSlaveRxCplt(I2CSeqHandle_t xI2CSeq) {
    switch (ucI2CSeqTransferDirection(xI2CSeq)) {
    case I2C_DIRECTION_TRANSMIT:
      switch (xI2CSeqXferBytes(xI2CSeq)) {
        uint8_t *pcBuffer;
      case 3U:
        pcBuffer = pvI2CSeqBuffer(xI2CSeq);
        usFile[pcBuffer[0U] & 0x03U] = (pcBuffer[1U] << 8U) | pcBuffer[2U];
      }
    }
  }

  vI2CSlaveDeviceAddr(xTMP10x->xI2CSlave, xTMP10x->ucAddr, prvAddr);
  vI2CSlaveDeviceSlaveRxCplt(xTMP10x->xI2CSlave, xTMP10x->ucAddr,
                             prvSlaveRxCplt);
  uint32_t ulNotified;
  xTaskNotifyWait(0UL, tmp10xSTOP_NOTIFIED, &ulNotified, portMAX_DELAY);
  vI2CSlaveDeviceAddr(xTMP10x->xI2CSlave, xTMP10x->ucAddr, NULL);
  vI2CSlaveDeviceSlaveRxCplt(xTMP10x->xI2CSlave, xTMP10x->ucAddr, NULL);
  vTaskDelete(NULL);
}

TMP10xHandle_t xTMP10xCreate(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr) {
  TMP10xHandle_t xTMP10x = pvPortMalloc(sizeof(*xTMP10x));
  configASSERT(xTMP10x);
  xTMP10x->xTask = NULL;
  xTMP10x->xI2CSlave = xI2CSlave;
  xTMP10x->ucAddr = ucAddr;
  return xTMP10x;
}

BaseType_t xTMP10xStart(TMP10xHandle_t xTMP10x) {
  configASSERT(xTMP10x->xTask == NULL);
  char name[configMAX_TASK_NAME_LEN];
  snprintf(name, sizeof(name), "%s-%02x", tmp10xTASK_NAME, xTMP10x->ucAddr);
  return xTaskCreate(prvTMP10xTask, name, tmp10xSTACK_DEPTH, xTMP10x,
                     tmp10xPRIORITY, &xTMP10x->xTask);
}

void vTMP10xStop(TMP10xHandle_t xTMP10x) {
  configASSERT(xTMP10x->xTask);
  xTaskNotify(xTMP10x->xTask, tmp10xSTOP_NOTIFIED, eSetBits);
  xTMP10x->xTask = NULL;
}

void vTMP10xDelete(TMP10xHandle_t xTMP10x) { vPortFree(xTMP10x); }
