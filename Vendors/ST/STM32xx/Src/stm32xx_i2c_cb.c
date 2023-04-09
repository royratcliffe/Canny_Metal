/*
 * stm32xx_i2c_cb.c
 */

#include "stm32xx_i2c_cb.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "stm32xx_registered_i2c.h"

#include "list_ex.h"

static List_t xMasterTxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xMasterRxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xSlaveTxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xSlaveRxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xAddrForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xListenCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xErrorForI2C[stm32xx_i2cMAX_INSTANCES];
static List_t xAbortForI2C[stm32xx_i2cMAX_INSTANCES];

static void __attribute__((constructor)) prvListInitialise() {
  for (size_t xCardinal = 0U; xCardinal < stm32xx_i2cMAX_INSTANCES; xCardinal++) {
    vListInitialise(xMasterTxCpltForI2C + xCardinal);
    vListInitialise(xMasterRxCpltForI2C + xCardinal);
    vListInitialise(xSlaveTxCpltForI2C + xCardinal);
    vListInitialise(xSlaveRxCpltForI2C + xCardinal);
    vListInitialise(xAddrForI2C + xCardinal);
    vListInitialise(xListenCpltForI2C + xCardinal);
    vListInitialise(xErrorForI2C + xCardinal);
    vListInitialise(xAbortForI2C + xCardinal);
  }
}

/*!
 * \defgroup halI2C_MasterTxCpltCallback Master Transmit Complete Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvMasterTxCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_MasterTxCpltCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xMasterTxCpltForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterMasterTxCpltHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xMasterTxCpltForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, prvMasterTxCplt);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_MasterRxCpltCallback Master Receive Complete Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvMasterRxCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_MasterRxCpltCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xMasterRxCpltForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterMasterRxCpltHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xMasterRxCpltForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, prvMasterRxCplt);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_SlaveTxCpltCallback Slave Transmit Complete Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvSlaveTxCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_SlaveTxCpltCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xSlaveTxCpltForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterSlaveTxCpltHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xSlaveTxCpltForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, prvSlaveTxCplt);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_SlaveRxCpltCallback Slave Receive Complete Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvSlaveRxCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_SlaveRxCpltCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xSlaveRxCpltForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterSlaveRxCpltHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xSlaveRxCpltForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, prvSlaveRxCplt);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_AddrCallback Address Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvAddr(I2CHandle_t xI2C, uint8_t ucTransferDirection, uint16_t usAddrMatchCode)
#else
void HAL_I2C_AddrCallback(I2CHandle_t xI2C, uint8_t ucTransferDirection, uint16_t usAddrMatchCode)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CAddrHandler_t)pxOwner)(xI2C, ucTransferDirection, usAddrMatchCode);
    return pdPASS;
  }
  vListYield(xAddrForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterAddrHandler(I2CHandle_t xI2C, I2CAddrHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xAddrForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterAddrCallback(xI2C, prvAddr);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_ListenCpltCallback Listen Complete Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvListenCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_ListenCpltCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xListenCpltForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterListenCpltHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xListenCpltForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_LISTEN_COMPLETE_CB_ID, prvListenCplt);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_ErrorCallback Error Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvError(I2CHandle_t xI2C)
#else
void HAL_I2C_ErrorCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xErrorForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterErrorHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xErrorForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_ERROR_CB_ID, prvError);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

/*!
 * \}
 */

/*!
 * \defgroup halI2C_AbortCallback Abort Callback
 * \{
 */

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvAbort(I2CHandle_t xI2C)
#else
void HAL_I2C_AbortCallback(I2CHandle_t xI2C)
#endif
{
  BaseType_t xYield(void *pxOwner, TickType_t xValue) {
    ((I2CHandler_t)pxOwner)(xI2C);
    return pdPASS;
  }
  vListYield(xAbortForI2C + xRegisteredCardinalOfI2C(xI2C), xYield);
}

ListItem_t *pxI2CRegisterAbortHandler(I2CHandle_t xI2C, I2CHandler_t xHandler, TickType_t xDelay) {
  List_t *pxForI2C = xAbortForI2C + xRegisteredCardinalOfI2C(xI2C);
#if USE_HAL_I2C_REGISTER_CALLBACKS
  if (listLIST_IS_EMPTY(pxForI2C)) HAL_I2C_RegisterCallback(xI2C, HAL_I2C_ABORT_CB_ID, prvAbort);
#endif
  return pxListInsertNew(pxForI2C, xHandler, xDelay);
}

void vI2CUnregister(ListItem_t *pxRegistered) {
  (void)uxListRemove(pxRegistered);
  vPortFree(pxRegistered);
}

/*!
 * \}
 */

#endif
