/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file core_dwt_cyc_clock64.c
 */
#include <core_dwt_cyc_clock64.h>
#include <stm32xx.h>

static volatile uint32_t ulTick = 0UL;
static volatile uint64_t ullNow = 0ULL;

static uint64_t now() {
  uint32_t tick = DWT->CYCCNT;
  if (tick < ulTick) {
    /*
     * Tick counter overflow. The +1 accounts for the counter rolling over to 0,
     * so the ticks between the last and current counter value equal
     * (UINT32_MAX - ulTick + tick + 1). For example, if ulTick is UINT32_MAX
     * and tick is 0, the elapsed ticks equal 1, not 0.
     */
    ullNow += UINT32_MAX - ulTick + tick + 1;
  } else {
    ullNow += tick - ulTick;
  }
  ulTick = tick;
  return ullNow;
}

static uint64_t ticks_per_us() { return SystemCoreClock / 1000000UL; }

/*
 * 64-bit clock using DWT cycle counter; "now" latches elapsed ticks;
 * "ticks_per_us" returns ticks per microsecond; const prevents modification and
 * places the implementation in flash memory.
 */
static const struct clock64_impl dwt_cyc_clock64_impl = {.now = now, .ticks_per_us = ticks_per_us};

static struct clock64 dwt_cyc_clock64 = {.impl = &dwt_cyc_clock64_impl};

void core_dwt_cyc_clock64_init(void) {
  /*
   * Enable the DWT cycle counter. This is necessary for the clock to function
   * correctly, as it relies on the DWT cycle counter to provide the tick count.
   * The DWT cycle counter is typically disabled by default, so it must be
   * explicitly enabled before using this clock.
   */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;                     /* Reset the cycle counter to start counting from 0. */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; /* Enable the cycle counter. */
}

struct clock64 *core_dwt_cyc_clock64(void) { return &dwt_cyc_clock64; }
