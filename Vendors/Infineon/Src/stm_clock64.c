#include <stm_clock64.h>
#include <when.h>

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

CAUSES(stm0_clock_sync, sync0); /* @suppress("Unused static function") @suppress("Unused variable declaration in file scope") */
CAUSES(stm1_clock_sync, sync1); /* @suppress("Unused static function") @suppress("Unused variable declaration in file scope") */
CAUSES(stm2_clock_sync, sync2); /* @suppress("Unused static function") @suppress("Unused variable declaration in file scope") */

static void sync0(void *with, ...) {
  (void)with;
  /*
   * Be careful with the following. The TASKING compiler generates code
   * in Debug mode even though voided. In Debug mode, the code is
   * generated and the argument is evaluated, so it must be safe to
   * evaluate. The argument is not used, so it can be anything.
   *
   *    (void)va_arg(*(va_list *)with, int);
   */
  clock64_sync(&stm0_tim);
}

static void sync1(void *with, ...) {
  (void)with;
  clock64_sync(&stm1_tim);
}

static void sync2(void *with, ...) {
  (void)with;
  clock64_sync(&stm2_tim);
}
