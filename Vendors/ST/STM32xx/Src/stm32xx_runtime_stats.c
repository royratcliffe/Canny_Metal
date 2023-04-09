/*
 * stm32xx_runtime_stats.c
 */

#include "stm32xx_mcu.h"

/*!
 * \file
 * \brief Functions needed when \c configGENERATE_RUN_TIME_STATS is on.
 */

__weak void configureTimerForRunTimeStats(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
}

__weak unsigned long getRunTimeCounterValue(void) { return DWT->CYCCNT; }
