/*
 * stm32xx_i2c_slave.c
 */

#include "stm32xx_i2c_slave.h"

#include "stm32xx_i2c_cb.h"
#include "stm32xx_i2c_seq.h"
#include "stm32xx_registered_i2c.h"

#include "task.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

struct I2CDevice {
  void (*pvSlaveTxCplt)(I2CSeqHandle_t xI2CSeq);
  void (*pvSlaveRxCplt)(I2CSeqHandle_t xI2CSeq);
  void (*pvAddr)(I2CSeqHandle_t xI2CSeq);
  void (*pvError)(I2CSeqHandle_t xI2CSeq);
};

/*!
 * \brief I2C slave internal structure.
 *
 * Persists the I2C handle, the slave task handle and its virtual I2C devices.
 */
struct I2CSlave {
  I2CHandle_t xI2C;
  TaskHandle_t xTask;
  struct I2CDevice *pxDevices[1U << 7U];
};

#define SLAVE_TX_CPLT_NOTIFIED (1UL << 0U)
#define SLAVE_RX_CPLT_NOTIFIED (1UL << 1U)
#define ADDR_NOTIFIED (1UL << 2U)
#define ERROR_NOTIFIED (1UL << 3U)
#define STOP_NOTIFIED (1UL << 31U)

/*!
 * Slave mode is straightforward: listen, receive then transmit, repeat forever.
 */
static portTASK_FUNCTION(prvI2CSlaveTask, pvParameters) {
  I2CSlaveHandle_t xI2CSlave = pvParameters;

  /*
   * Reuse an I2C sequencer. Sets up the transfer. Transmit and receive
   * implementations capture the slave context. There can be only one transfer
   * at any one time.
   *
   * Set up the sequencer for interrupt-driven slave transfers.
   */
  I2CSeqHandle_t xI2CSeq = xI2CSeqCreate(xI2CSlave->xI2C);
  vI2CSeqSlaveIT(xI2CSeq);

  /*
   * Capture slave transmit complete, slave receive complete, listen complete,
   * address and error.
   *
   * Bounce the interrupt signals to the slave task. This has an important side
   * effect. The interrupts trigger a task-time response and the device relay
   * runs at within a task rather than within an interrupt service routine.
   */
  TaskHandle_t xTask = xTaskGetCurrentTaskHandle();

  void prvSlaveTxCplt(I2CHandle_t xI2C) {
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, SLAVE_TX_CPLT_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvSlaveRxCplt(I2CHandle_t xI2C) {
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, SLAVE_RX_CPLT_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvListenCplt(I2CHandle_t xI2C) {
    HAL_StatusTypeDef xStatus = HAL_I2C_EnableListen_IT(xI2C);
    configASSERT(xStatus == HAL_OK);
  }

  /*
   * The transfer direction relates to the master. Transmit direction means that
   * the master wants to transmit and therefore the slave needs to receive.
   *
   * The implementation captures the I2C sequencer and updates its transfer
   * direction and address. The updating runs at interrupt time.
   */
  void prvAddr(I2CHandle_t xI2C, uint8_t ucTransferDirection,
               uint16_t usAddrMatchCode) {
    vI2CSeqTransferDirection(xI2CSeq, ucTransferDirection);
    vI2CSeqAddr(xI2CSeq, usAddrMatchCode >> 1U);
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, ADDR_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvError(I2CHandle_t xI2C) {
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, ERROR_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  ListItem_t *pxSlaveTxCplt = pxI2CRegisterSlaveTxCpltHandler(
      xI2CSlave->xI2C, prvSlaveTxCplt, portMAX_DELAY);
  ListItem_t *pxSlaveRxCplt = pxI2CRegisterSlaveRxCpltHandler(
      xI2CSlave->xI2C, prvSlaveRxCplt, portMAX_DELAY);
  ListItem_t *pxListenCplt = pxI2CRegisterListenCpltHandler(
      xI2CSlave->xI2C, prvListenCplt, portMAX_DELAY);
  ListItem_t *pxAddr =
      pxI2CRegisterAddrHandler(xI2CSlave->xI2C, prvAddr, portMAX_DELAY);
  ListItem_t *pxError =
      pxI2CRegisterErrorHandler(xI2CSlave->xI2C, prvError, portMAX_DELAY);

  /*
   * Wait indefinitely for stop notification.
   */
  if (HAL_I2C_EnableListen_IT(xI2CSlave->xI2C) == HAL_OK) {
    uint32_t ulNotified;
    do {
      xTaskNotifyWait(0UL, ULONG_MAX, &ulNotified, portMAX_DELAY);
      uint8_t ucAddr = ucI2CSeqAddr(xI2CSeq);
      struct I2CDevice *pxDevice = xI2CSlave->pxDevices[ucAddr];
      if (pxDevice) {
        if ((ulNotified & SLAVE_TX_CPLT_NOTIFIED) && pxDevice->pvSlaveTxCplt)
          pxDevice->pvSlaveTxCplt(xI2CSeq);
        if ((ulNotified & SLAVE_RX_CPLT_NOTIFIED) && pxDevice->pvSlaveRxCplt)
          pxDevice->pvSlaveRxCplt(xI2CSeq);
        if ((ulNotified & ADDR_NOTIFIED) && pxDevice->pvAddr)
          pxDevice->pvAddr(xI2CSeq);
        if ((ulNotified & ERROR_NOTIFIED) && pxDevice->pvError)
          pxDevice->pvError(xI2CSeq);
      }
    } while ((ulNotified & STOP_NOTIFIED) == 0UL);
  }

  /*
   * Tear down the slave. Remove all its handlers before deleting its task.
   * Do not forget to dispose of the sequencer.
   *
   * Always stop the slave *before* deleting it. Deleting frees the internal
   * structure and therefore avoid accessing the structure members after the
   * stop notification.
   */
  (void)uxListRemove(pxSlaveTxCplt);
  (void)uxListRemove(pxSlaveRxCplt);
  (void)uxListRemove(pxListenCplt);
  (void)uxListRemove(pxAddr);
  (void)uxListRemove(pxError);
  vI2CSeqDelete(xI2CSeq);
  vTaskDelete(NULL);
}

I2CSlaveHandle_t xI2CSlaveCreate(I2CHandle_t xI2C) {
  I2CSlaveHandle_t xI2CSlave = pvPortMalloc(sizeof(*xI2CSlave));
  configASSERT(xI2CSlave);
  (void)memset(xI2CSlave, 0, sizeof(*xI2CSlave));
  xI2CSlave->xI2C = xI2C;
  return xI2CSlave;
}

BaseType_t xI2CSlaveStart(I2CSlaveHandle_t xI2CSlave, const char *pcName) {
  configASSERT(xI2CSlave->xTask == NULL);
  char name[configMAX_TASK_NAME_LEN];
  snprintf(name, sizeof(name), "%s-I2C%d", pcName ? pcName : i2cslaveTASK_NAME,
           xRegisteredCardinalOfI2C(xI2CSlave->xI2C));
  return xTaskCreate(prvI2CSlaveTask, name, i2cslaveSTACK_DEPTH, xI2CSlave,
                     i2cslavePRIORITY, &xI2CSlave->xTask);
}

void vI2CSlaveStop(I2CSlaveHandle_t xI2CSlave) {
  configASSERT(xI2CSlave->xTask);
  xTaskNotify(xI2CSlave->xTask, STOP_NOTIFIED, eSetBits);
  xI2CSlave->xTask = NULL;
}

void vI2CSlaveDelete(I2CSlaveHandle_t xI2CSlave) {
  for (uint8_t ucAddr = 0x00U;
       ucAddr < sizeof(xI2CSlave->pxDevices) / sizeof(xI2CSlave->pxDevices[0]);
       ucAddr++)
    vPortFree(xI2CSlave->pxDevices[ucAddr]);
  vPortFree(xI2CSlave);
}

/*!
 * \param[in] ucAddr 7-bit slave address of virtual device without shift.
 *
 * Constructs the slave device lazily. The slave is an association entity
 * linking a slave with its bus address.
 *
 * \note Watch out for race conditions.
 */
static struct I2CDevice *prvI2CSlaveDevice(struct I2CSlave *pxI2CSlave,
                                           uint8_t ucAddr) {
  configASSERT((ucAddr & 0b10000000U) == 0b00000000U);
  struct I2CDevice *pxDevice = pxI2CSlave->pxDevices[ucAddr];
  if (pxDevice == NULL) {
    pxDevice = pvPortMalloc(sizeof(*pxDevice));
    configASSERT(pxDevice);
    pxI2CSlave->pxDevices[ucAddr] = pxDevice;
  }
  return pxDevice;
}

void vI2CSlaveDeviceSlaveTxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveTxCplt)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvSlaveTxCplt = pvSlaveTxCplt;
}

void vI2CSlaveDeviceSlaveRxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveRxCplt)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvSlaveRxCplt = pvSlaveRxCplt;
}

void vI2CSlaveDeviceAddr(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                         void (*pvAddr)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvAddr = pvAddr;
}

void vI2CSlaveDeviceError(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                          void (*pvError)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvError = pvError;
}
