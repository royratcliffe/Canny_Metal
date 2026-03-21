/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file stm32xx_hal_tick_clock64.c
 * \brief Provides a 64-bit tick clock implementation based on the HAL tick.
 * \note This clock relies on the HAL tick, so it is important to ensure that
 * the HAL tick is properly configured and running for this clock to function
 * correctly. Additionally, since the HAL tick typically has a resolution of 1
 * millisecond, the \c ticks_per_us function will return a value that reflects
 * this resolution, which may affect the precision of timing calculations that
 * use this clock.
 */
#include <stm32xx_hal.h>

/*
 * There is no HAL_TICK_MODULE_ENABLED, but the HAL driver is only used if
 * USE_HAL_DRIVER is defined. Therefore check for that.
 */
#if defined(USE_HAL_DRIVER)

#  include <stm32xx_hal_tick_clock64.h>
#  include <stm32xx_hal.h>

static volatile uint32_t ulTick = 0UL;
static volatile uint64_t ullNow = 0ULL;

static uint64_t now() {
  uint32_t tick = HAL_GetTick();
  if (tick < ulTick) {
    /*
     * Tick counter overflowed, account for that. The +1 exists because the tick
     * counter rolls over to 0, so the number of ticks between the last tick and
     * the current tick is (UINT32_MAX - ulTick + tick + 1). For example, if
     * ulTick is UINT32_MAX and tick is 0, then the number of ticks between them
     * is 1, not 0.
     */
    ullNow += UINT32_MAX - ulTick + tick + 1;
  } else {
    ullNow += tick - ulTick;
  }
  ulTick = tick;
  return ullNow;
}

static uint64_t ticks_per_us() { return HAL_GetTickFreq() * 1000ULL; }

/*
 * Provide a 64-bit clock source based on the HAL tick. The "now" function
 * calculates elapsed ticks since the last call, whilst "ticks_per_us" returns
 * ticks per microsecond. The implementation is const to prevent modification
 * and to live in flash memory as a fixed configuration.
 */
static const struct clock64_impl hal_tick_clock64_impl = {.now = now, .ticks_per_us = ticks_per_us};

static struct clock64 hal_tick_clock64 = {.impl = &hal_tick_clock64_impl};

struct clock64 *stm32xx_hal_tick_clock64(void) { return &hal_tick_clock64; }

#endif /* USE_HAL_DRIVER */
