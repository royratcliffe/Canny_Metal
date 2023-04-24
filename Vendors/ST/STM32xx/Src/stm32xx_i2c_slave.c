// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "stm32xx_i2c_slave.h"

#include "stm32xx_i2c_cb.h"
#include "stm32xx_i2c_seq.h"
#include "stm32xx_registered_i2c.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "task.h"

#include "registered_opaques.h"

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
  void *pvAddrs[i2cslaveMAX_DEVICES];
  struct I2CDevice *pxDevices[i2cslaveMAX_DEVICES];
};

static size_t prvHashOfAddr(void *pvOpaque) { return (size_t)pvOpaque; }

/*!
 * \brief Registers address.
 *
 * Merges the address with an arbitrary non-zero bits to allow for address zero.
 * In practice, address zero is the "general call" address and not used.
 * Registered opaque pointers rely on NULL, or all zero, for empty registration
 * slots. Avoid stepping on the registration semantics.
 */
static size_t prvRegisteredCardinalOfAddr(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr) {
  struct RegisteredOpaques prvRegisteredOpaques = {.ppvOpaques = xI2CSlave->pvAddrs,
                                                   .xNumberOfOpaques = i2cslaveMAX_DEVICES,
                                                   .pxHashOfOpaqueFunction = prvHashOfAddr};
  return xRegisteredCardinalOfOpaque(&prvRegisteredOpaques, (void *)(0xdeadbe00UL | ucAddr));
}

static struct I2CDevice **prvRegisteredDeviceOfAddr(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr) {
  return xI2CSlave->pxDevices + prvRegisteredCardinalOfAddr(xI2CSlave, ucAddr);
}

#define i2cslaveSLAVE_TX_CPLT_NOTIFIED (1UL << ('T' - '@'))
#define i2cslaveSLAVE_RX_CPLT_NOTIFIED (1UL << ('R' - '@'))
#define i2cslaveLISTEN_CPLT_NOTIFIED   (1UL << ('L' - '@'))
#define i2cslaveADDR_NOTIFIED          (1UL << ('A' - '@'))
#define i2cslaveERROR_NOTIFIED         (1UL << ('E' - '@'))
#define i2cslaveSTOP_NOTIFIED          (1UL << ('_' - '@'))

/*!
 * \brief I2C slave service code.
 *
 * Slave mode is straightforward: listen, receive then transmit, repeat forever.
 *
 * The task quits immediately if I2C listening fails to enable. This happens if
 * the channel has not been correctly initialised.
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
    (void)xI2C;
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, i2cslaveSLAVE_TX_CPLT_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvSlaveRxCplt(I2CHandle_t xI2C) {
    (void)xI2C;
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, i2cslaveSLAVE_RX_CPLT_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvListenCplt(I2CHandle_t xI2C) {
    (void)xI2C;
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, i2cslaveLISTEN_CPLT_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  /*
   * The transfer direction relates to the master. Transmit direction means that
   * the master wants to transmit and therefore the slave needs to receive.
   *
   * The implementation captures the I2C sequencer and updates its transfer
   * direction and address. The updating runs at interrupt time. This makes some
   * important assumptions about the asynchronous transfer process. It assumes
   * that the "address" event never overlaps a transfer in progress which
   * therefore implies that the sequencer is not currently in use: a normal and
   * fair assumption to make given that I2C transfers never overlap on the bus.
   */
  void prvAddr(I2CHandle_t xI2C, uint8_t ucTransferDirection, uint16_t usAddrMatchCode) {
    (void)xI2C;
    vI2CSeqTransferDirection(xI2CSeq, ucTransferDirection);
    vI2CSeqAddr(xI2CSeq, usAddrMatchCode >> 1U);
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, i2cslaveADDR_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  void prvError(I2CHandle_t xI2C) {
    (void)xI2C;
    BaseType_t xWoken;
    xTaskNotifyFromISR(xTask, i2cslaveERROR_NOTIFIED, eSetBits, &xWoken);
    portYIELD_FROM_ISR(xWoken);
  }

  ListItem_t *pxSlaveTxCplt = pxI2CRegisterSlaveTxCpltHandler(xI2CSlave->xI2C, prvSlaveTxCplt, portMAX_DELAY);
  ListItem_t *pxSlaveRxCplt = pxI2CRegisterSlaveRxCpltHandler(xI2CSlave->xI2C, prvSlaveRxCplt, portMAX_DELAY);
  ListItem_t *pxListenCplt = pxI2CRegisterListenCpltHandler(xI2CSlave->xI2C, prvListenCplt, portMAX_DELAY);
  ListItem_t *pxAddr = pxI2CRegisterAddrHandler(xI2CSlave->xI2C, prvAddr, portMAX_DELAY);
  ListItem_t *pxError = pxI2CRegisterErrorHandler(xI2CSlave->xI2C, prvError, portMAX_DELAY);

  /*
   * Wait indefinitely for stop notification.
   */
  if (HAL_I2C_EnableListen_IT(xI2CSlave->xI2C) == HAL_OK) {
    uint32_t ulNotified;
    do {
      xTaskNotifyWait(0UL, ULONG_MAX, &ulNotified, portMAX_DELAY);
      uint8_t ucAddr = ucI2CSeqAddr(xI2CSeq);
      struct I2CDevice *pxDevice = *prvRegisteredDeviceOfAddr(xI2CSlave, ucAddr);
      if (pxDevice) {
        if ((ulNotified & i2cslaveSLAVE_TX_CPLT_NOTIFIED) && pxDevice->pvSlaveTxCplt) pxDevice->pvSlaveTxCplt(xI2CSeq);
        if ((ulNotified & i2cslaveSLAVE_RX_CPLT_NOTIFIED) && pxDevice->pvSlaveRxCplt) pxDevice->pvSlaveRxCplt(xI2CSeq);
        if ((ulNotified & i2cslaveADDR_NOTIFIED) && pxDevice->pvAddr) pxDevice->pvAddr(xI2CSeq);
        if ((ulNotified & i2cslaveERROR_NOTIFIED) && pxDevice->pvError) pxDevice->pvError(xI2CSeq);
      } else {
        /*
         * Perform a dummy transfer when no device exists. Always try at least
         * one frame. This may fail if the master fails to acknowledge such as
         * when the master performs a device address ping using
         * HAL_I2C_IsDeviceReady. Set up the transfer nevertheless. Adjust the
         * slave mask to avoid overlap with non-emulated peripherals sharing the
         * same bus.
         */
        if ((ulNotified & i2cslaveADDR_NOTIFIED)) {
          vI2CSeqBufferLength(xI2CSeq, 1UL);
          *((uint8_t *)pvI2CSeqBuffer(xI2CSeq)) = 0xa5U;
          xI2CSeqLastFrame(xI2CSeq);
        }
      }
      if ((ulNotified & i2cslaveLISTEN_CPLT_NOTIFIED)) HAL_I2C_EnableListen_IT(xI2CSlave->xI2C);
    } while ((ulNotified & i2cslaveSTOP_NOTIFIED) == 0UL);
  }

  /*
   * Tear down the slave. Remove all its handlers before deleting its task.
   * Do not forget to dispose of the sequencer.
   *
   * Always stop the slave *before* deleting it. Deleting frees the internal
   * structure and therefore avoid accessing the structure members after the
   * stop notification.
   */
  vI2CUnregister(pxSlaveTxCplt);
  (void)vI2CUnregister(pxSlaveRxCplt);
  (void)vI2CUnregister(pxListenCplt);
  (void)vI2CUnregister(pxAddr);
  (void)vI2CUnregister(pxError);
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
  /*
   * Use hexadecimal and two digits for the I2C cardinal in order not to confuse
   * the cardinal with the I2C channel. The two spaces do not directly connect,
   * only indirectly through the I2C registered hashing function.
   */
  snprintf(name, sizeof(name), "%s-I2C%02x", pcName ? pcName : i2cslaveTASK_NAME,
           xRegisteredCardinalOfI2C(xI2CSlave->xI2C));
  return xTaskCreate(prvI2CSlaveTask, name, i2cslaveSTACK_DEPTH, xI2CSlave, i2cslavePRIORITY, &xI2CSlave->xTask);
}

void vI2CSlaveStop(I2CSlaveHandle_t xI2CSlave) {
  configASSERT(xI2CSlave->xTask);
  xTaskNotify(xI2CSlave->xTask, i2cslaveSTOP_NOTIFIED, eSetBits);
  xI2CSlave->xTask = NULL;
}

void vI2CSlaveDelete(I2CSlaveHandle_t xI2CSlave) {
  for (uint8_t ucAddr = 0x00U; ucAddr < sizeof(xI2CSlave->pxDevices) / sizeof(xI2CSlave->pxDevices[0]); ucAddr++)
    vPortFree(xI2CSlave->pxDevices[ucAddr]);
  vPortFree(xI2CSlave);
}

/*!
 * \param[in] ucAddr 7-bit slave address of virtual device without shift.
 *
 * Constructs the slave device lazily. The slave is an association entity
 * linking a slave with its bus address.
 *
 * \note Watch out for race conditions. Best to set up devices \e before
 * starting the slave.
 */
static struct I2CDevice *prvI2CSlaveDevice(struct I2CSlave *pxI2CSlave, uint8_t ucAddr) {
  configASSERT((ucAddr & 0b10000000U) == 0b00000000U);
  struct I2CDevice **ppxDevice = prvRegisteredDeviceOfAddr(pxI2CSlave, ucAddr);
  if (ppxDevice == NULL) {
    *ppxDevice = pvPortMalloc(sizeof(**ppxDevice));
    configASSERT(*ppxDevice);
    (void)memset(*ppxDevice, 0, sizeof(**ppxDevice));
  }
  return *ppxDevice;
}

void vI2CSlaveDeviceSlaveTxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveTxCplt)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvSlaveTxCplt = pvSlaveTxCplt;
}

void vI2CSlaveDeviceSlaveRxCplt(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr,
                                void (*pvSlaveRxCplt)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvSlaveRxCplt = pvSlaveRxCplt;
}

void vI2CSlaveDeviceAddr(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr, void (*pvAddr)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvAddr = pvAddr;
}

void vI2CSlaveDeviceError(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddr, void (*pvError)(I2CSeqHandle_t xI2CSeq)) {
  prvI2CSlaveDevice(xI2CSlave, ucAddr)->pvError = pvError;
}

#endif // ifdef HAL_I2C_MODULE_ENABLED
