/*
 * stm32xx_i2c_mutex.c FreeRTOS STM32xx Drivers
 */

#include "stm32xx_i2c_mutex.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include "semphr.h"

#if configSUPPORT_STATIC_ALLOCATION
static StaticSemaphore_t xStaticSemaphores[stm32xx_i2cMAX_INSTANCES];
#endif

static SemaphoreHandle_t xSemaphores[stm32xx_i2cMAX_INSTANCES];

BaseType_t xI2CGive(I2CHandle_t xI2C)
{
	SemaphoreHandle_t xSemaphore = xSemaphores[ucCardinalForI2C(xI2C->Instance)];
	return xSemaphore ? xSemaphoreGive(xSemaphore) : pdFAIL;
}

BaseType_t xI2CTake(I2CHandle_t xI2C, TickType_t xTicksToWait)
{
	SemaphoreHandle_t xSemaphore = xSemaphores[ucCardinalForI2C(xI2C->Instance)];
	return xSemaphore ? xSemaphoreTake(xSemaphore, xTicksToWait) : pdFAIL;
}

void vI2CCreateRecursiveMutex(I2CHandle_t xI2C)
{
	uint8_t ucCardinal = ucCardinalForI2C(xI2C->Instance);
	if (xSemaphores[ucCardinal] != NULL) return;
#if configSUPPORT_STATIC_ALLOCATION
	xSemaphores[ucCardinal] = xSemaphoreCreateRecursiveMutexStatic(&xStaticSemaphores[ucCardinal]);
#else
	xSemaphores[ucCardinal] = xSemaphoreCreateRecursiveMutex();
	configASSERT(xSemaphores[ucCardinal]);
#endif
}

#endif // ifdef HAL_I2C_MODULE_ENABLED
