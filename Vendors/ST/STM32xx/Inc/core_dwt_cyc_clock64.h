/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file core_dwt_cyc_clock64.h
 */
#ifndef CORE_DWT_CYC_CLOCK64_H
#define CORE_DWT_CYC_CLOCK64_H

#include <clock64.h>

/*!
 * \brief Initialises the DWT cycle counter for use as a 64-bit clock source.
 * \details This function enables the DWT cycle counter, which is necessary for
 * the clock to function correctly. The DWT cycle counter is typically disabled
 * by default, so it must be explicitly enabled before using this clock. This
 * function should be called before using the \c core_dwt_cyc_clock64 function to
 * ensure that the DWT cycle counter is properly configured and ready to provide
 * tick counts for the clock.
 * \note The DWT cycle counter is a hardware feature of the Cortex-M4 that
 * provides a high-resolution tick count based on the number of CPU cycles.
 * Enabling the DWT cycle counter allows the \c core_dwt_cyc_clock64
 * implementation to provide accurate timing information based on the CPU's
 * clock frequency, making it suitable for performance measurements and precise
 * timing applications.
 */
void core_dwt_cyc_clock64_init(void);

/*!
 * \brief A 64-bit tick clock implementation based on the DWT cycle counter.
 * \details This clock uses the DWT cycle counter as its time base. It provides a
 * 64-bit tick count for timing and scheduling. The \c now function calculates
 * the current tick count by accounting for DWT cycle counter overflow, ensuring
 * a continuous and accurate count over long periods. The \c ticks_per_us function
 * returns the number of ticks per microsecond based on DWT cycle counter frequency,
 * enabling precise timing calculations. This clock serves as a root clock in a
 * clock hierarchy, providing a reliable time base for derived clocks.
 * \note This clock requires a properly configured and running DWT cycle counter.
 * Since the DWT cycle counter typically has 1 microsecond resolution, the
 * \c ticks_per_us function returns a value reflecting this resolution, which
 * affects the precision of timing calculations.
 */
struct clock64 *core_dwt_cyc_clock64(void);

#endif /* CORE_DWT_CYC_CLOCK64_H */
