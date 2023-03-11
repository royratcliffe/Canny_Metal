/*
 * stm32xx_i2c_slave.c
 *
 *  Created on: Mar 11, 2023
 *      Author: RoyRatcliffe
 */

#include "stm32xx_i2c_slave.h"

#include "task.h"

#include <string.h>

struct I2CSlave {
	I2CHandle_t xI2C;
	HAL_StatusTypeDef (*pxReceive)(I2CHandle_t xI2C, void *pvRxBuffer, size_t xBufferLengthBytes);
	HAL_StatusTypeDef (*pxTransmit)(I2CHandle_t xI2C, const void *pvTxData, size_t xDataLengthBytes);
	TaskHandle_t xTask;
	I2CSlaveHandler_t xHandlers[1U << 7U];
};

/*!
 * Slave mode is straightforward: listen, receive then transmit, repeat forever.
 */
static portTASK_FUNCTION(prvI2CSlaveTask, pvParameters)
{
	struct I2CSlave *pxI2CSlave = pvParameters;
	for (;;)
	{
		if (HAL_I2C_EnableListen_IT(pxI2CSlave->xI2C) != HAL_OK);
		uint8_t ucBuffer[i2cslaveBUFFER_LENGTH_BYTES];
		if (pxI2CSlave->pxReceive(pxI2CSlave->xI2C, ucBuffer, i2cslaveBUFFER_LENGTH_BYTES) != HAL_OK)
		{
			;
		}
		/*
		 * Decode the address and capture the receiver buffer usage, i.e. the length of the incoming I2C frame.
		 * Listening to the I2C channel gives the address.
		 */
		void *pvTxData = NULL;
		size_t xDataLengthBytes = 0UL;
		I2CSlaveHandler_t xHandler = pxI2CSlave->xHandlers[0x00];
		if (xHandler) xDataLengthBytes = xHandler(ucBuffer, 2UL, &pvTxData);
		while (HAL_I2C_GetState(pxI2CSlave->xI2C) != HAL_I2C_STATE_READY);
		if (pxI2CSlave->pxTransmit(pxI2CSlave->xI2C, ucBuffer, xDataLengthBytes) != HAL_OK)
		{
			;
		}
		if (pvTxData) vPortFree(pvTxData);
		while (HAL_I2C_GetState(pxI2CSlave->xI2C) != HAL_I2C_STATE_READY);
	}
}

static HAL_StatusTypeDef prvReceiveIT(I2CHandle_t xI2C, void *pvRxBuffer, size_t xBufferLengthBytes)
{
	return HAL_I2C_Slave_Receive_IT(xI2C, pvRxBuffer, xBufferLengthBytes);
}

static HAL_StatusTypeDef prvTransmitIT(I2CHandle_t xI2C, const void *pvTxData, size_t xDataLengthBytes)
{
	return HAL_I2C_Slave_Transmit_IT(xI2C, (void *)pvTxData, xDataLengthBytes);
}

I2CSlaveHandle_t xI2CSlaveCreate(I2CHandle_t xI2C)
{
	struct I2CSlave *pxI2CSlave = pvPortMalloc(sizeof(*pxI2CSlave));
	configASSERT(pxI2CSlave);
	pxI2CSlave->xI2C = xI2C;
	pxI2CSlave->pxReceive = prvReceiveIT;
	pxI2CSlave->pxTransmit = prvTransmitIT;
	return pxI2CSlave;
}

BaseType_t xI2CSlaveStart(I2CSlaveHandle_t xI2CSlave, const char *pcName)
{
	struct I2CSlave *pxI2CSlave = xI2CSlave;
	configASSERT(pxI2CSlave->xTask == NULL);
	return xTaskCreate(prvI2CSlaveTask, pcName ? pcName : i2cslaveNAME, i2cslaveSTACK_DEPTH, xI2CSlave, i2cslavePRIORITY, &pxI2CSlave->xTask);
}

void vI2CSlaveStop(I2CSlaveHandle_t xI2CSlave)
{
	struct I2CSlave *pxI2CSlave = xI2CSlave;
	configASSERT(pxI2CSlave->xTask != NULL);
	vTaskDelete(pxI2CSlave->xTask);
	pxI2CSlave->xTask = NULL;
}

static HAL_StatusTypeDef prvReceiveDMA(I2CHandle_t xI2C, void *pvRxBuffer, size_t xBufferLengthBytes)
{
	return HAL_I2C_Slave_Receive_DMA(xI2C, pvRxBuffer, xBufferLengthBytes);
}

/*
 * The underlying hardware abstraction layer fails to apply `const` correctness to the transmit data pointer.
 * It does not modify the transmit bytes.
 */
static HAL_StatusTypeDef prvTransmitDMA(I2CHandle_t xI2C, const void *pvTxData, size_t xDataLengthBytes)
{
	return HAL_I2C_Slave_Transmit_DMA(xI2C, (void *)pvTxData, xDataLengthBytes);
}

void vI2CSlaveUseDMA(I2CSlaveHandle_t xI2CSlave)
{
	struct I2CSlave *pxI2CSlave = xI2CSlave;
	pxI2CSlave->pxReceive = prvReceiveDMA;
	pxI2CSlave->pxTransmit = prvTransmitDMA;
}

void vI2CSlaveAddressHandler(I2CSlaveHandle_t xI2CSlave, uint8_t ucAddress, I2CSlaveHandler_t xHandler)
{
	configASSERT(ucAddress < (1U << 7U));
	struct I2CSlave *pxI2CSlave = xI2CSlave;
	pxI2CSlave->xHandlers[ucAddress] = xHandler;
}
