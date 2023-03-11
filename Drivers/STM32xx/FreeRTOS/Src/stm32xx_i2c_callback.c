/*
 * stm32xx_i2c_callback.c
 *
 *  Created on: Mar 11, 2023
 *      Author: roy.ratcliffe
 */

#include "stm32xx_i2c_callback.h"

#include "hand_side.h"

static struct hand_side *pSlaveTxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static struct hand_side *pSlaveRxCpltForI2C[stm32xx_i2cMAX_INSTANCES];
static struct hand_side *pAddrForI2C[stm32xx_i2cMAX_INSTANCES];
static struct hand_side *pListenCpltForI2C[stm32xx_i2cMAX_INSTANCES];

#if USE_HAL_I2C_REGISTER_CALLBACKS
static void prvSlaveTxCplt(I2CHandle_t xI2C)
#else
void HAL_I2C_SlaveTxCpltCallback(I2CHandle_t xI2C)
#endif
{
	struct hand_side **ppSlaveTxCpltForI2C = pSlaveTxCpltForI2C + ucCardinalForI2C(xI2C->Instance);
	for (struct hand_side *pop = *ppSlaveTxCpltForI2C; pop; pop = pop->pop)
		if (pop->left == xI2C) (*(I2CHandler_t)pop->right)(xI2C);
}

void HAL_I2C_SlaveRxCpltCallback(I2CHandle_t xI2C)
{
	;
}

void HAL_I2C_AddrCallback(I2CHandle_t xI2C, uint8_t ucTransferDirection, uint16_t usAddrMatchCode)
{
	;
}

void HAL_I2C_ListenCpltCallback(I2CHandle_t xI2C)
{
	;
}

void vI2CRegisterSlaveTxCplt(I2CHandle_t xI2C, I2CHandler_t xHandler)
{
	struct hand_side **ppSlaveTxCpltForI2C = pSlaveTxCpltForI2C + ucCardinalForI2C(xI2C->Instance);
#if USE_HAL_I2C_REGISTER_CALLBACKS
	if (*ppSlaveTxCpltForI2C == NULL) HAL_I2C_RegisterSlaveTxCpltCallback(xI2C, prvSlaveTxCplt);
#endif
	push_hand_side(ppSlaveTxCpltForI2C, xI2C, xHandler);
}
