#include <clock64.h>

/*!
 * \brief Provides a 64-bit tick clock implementation based on the HAL tick.
 * \details This clock implementation uses the HAL tick as its time base,
 * allowing it to provide a 64-bit tick count that can be used for timing and
 * scheduling purposes. The clock's \c now function calculates the current tick
 * count by accounting for any overflow of the HAL tick counter, ensuring that
 * it can provide a continuous and accurate tick count even over long periods of
 * time. The \c ticks_per_us function returns the number of ticks per
 * microsecond based on the HAL tick frequency, allowing for precise timing
 * calculations. This clock can be used as a root clock in a clock hierarchy,
 * providing a reliable time base for other clocks that may derive from it.
 * \note This clock relies on the HAL tick, so it is important to ensure that
 * the HAL tick is properly configured and running for this clock to function
 * correctly. Additionally, since the HAL tick typically has a resolution of 1
 * millisecond, the \c ticks_per_us function will return a value that reflects
 * this resolution, which may affect the precision of timing calculations that
 * use this clock.
 */
struct clock64 *stm32xx_hal_tick_clock64(void);
