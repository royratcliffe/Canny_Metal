#include <stm_clock64.h>

/*
 * for Infineon System Timer Module (STM) Low-Level (iLLD) Drivers
 */
#include "IfxStm.h"

/*!
 * \brief The number of ticks per microsecond for the STM timers.
 * \details This value is based on the clock frequency of the STM timers
 * and is used to convert the raw tick counts from the timers into
 * microseconds. The STM timers typically run at a frequency of 300 MHz,
 * which results in 300 ticks per microsecond. This constant is used in
 * the implementation of the clock64 functions to provide accurate time
 * measurements based on the STM timers. It is important to ensure that
 * this value matches the actual clock frequency of the STM timers being
 * used, as an incorrect value could lead to inaccurate time
 * measurements.
 */
#define STM_TIM_TICKS_PER_US 300ULL

/*
 * On TriCore with optimisation enabled, the "now" function for STM0
 * becomes a pair of word loads from the TIM0 and CAP registers, which
 * are then combined to form the 64-bit time value. The assembly code
 * for the stm0_tim_now function looks like this:
 *
 * stm0_tim_now:
 *      .type   func
 *      ld.w    d2,0xf0001010
 *      ld.w    d3,0xf000102c
 *      ret
 */
static uint64_t stm0_tim_now(void) { return IfxStm_get(&MODULE_STM0); }
static uint64_t stm1_tim_now(void) { return IfxStm_get(&MODULE_STM1); }
static uint64_t stm2_tim_now(void) { return IfxStm_get(&MODULE_STM2); }

static uint64_t stm_tim_ticks_per_us(void) { return STM_TIM_TICKS_PER_US; }

static const struct clock64_impl stm0_tim_impl = {.now = stm0_tim_now, .ticks_per_us = stm_tim_ticks_per_us};
static const struct clock64_impl stm1_tim_impl = {.now = stm1_tim_now, .ticks_per_us = stm_tim_ticks_per_us};
static const struct clock64_impl stm2_tim_impl = {.now = stm2_tim_now, .ticks_per_us = stm_tim_ticks_per_us};

static struct clock64 stm0_tim = {.impl = &stm0_tim_impl};
static struct clock64 stm1_tim = {.impl = &stm1_tim_impl};
static struct clock64 stm2_tim = {.impl = &stm2_tim_impl};

struct clock64 *STM0_TIM_clock64(void) { return &stm0_tim; }

struct clock64 *STM1_TIM_clock64(void) { return &stm1_tim; }

struct clock64 *STM2_TIM_clock64(void) { return &stm2_tim; }
