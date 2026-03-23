#include <stm_clock64.h>
#include <when.h>

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
  clock64_sync(STM0_TIM_clock64());
}

static void sync1(void *with, ...) {
  (void)with;
  clock64_sync(STM1_TIM_clock64());
}

static void sync2(void *with, ...) {
  (void)with;
  clock64_sync(STM2_TIM_clock64());
}
