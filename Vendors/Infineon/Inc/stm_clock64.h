/*!
 * \file stm_clock64.h
 * \brief Header file for the STM clock64 implementation.
 * \details This header file declares the functions for accessing the
 * clock64 instances for the Infineon System Timer Module (STM) timers.
 * Each timer has its own clock64 instance, which provides the current
 * time and ticks per microsecond based on the STM's hardware timer. The
 * clock64 instances are designed to be used in a hierarchical clock
 * structure, allowing them to be synchronised with a root clock and to
 * have their own tick functions for responding to time changes. This
 * design allows for a flexible and extensible clock system that can be
 * easily integrated into a larger application.
 */
#ifndef STM_CLOCK64_H
#define STM_CLOCK64_H

#include <clock64.h>

/*!
 * \brief Get the clock64 instance for STM0 timer.
 * \return A pointer to the clock64 instance for STM0 timer.
 * \details This function returns a pointer to the clock64 instance that
 * represents the STM0 timer. The clock64 instance provides the current
 * time and ticks per microsecond based on the STM0 timer's hardware
 * timer. This function can be used to access the STM0 timer's clock64
 * instance for synchronisation and timekeeping purposes in a larger
 * application.
 */
struct clock64 *STM0_TIM_clock64(void);

/*!
 * \brief Get the clock64 instance for STM1 timer.
 * \return A pointer to the clock64 instance for STM1 timer.
 */
struct clock64 *STM1_TIM_clock64(void);

/*!
 * \brief Get the clock64 instance for STM2 timer.
 * \return A pointer to the clock64 instance for STM2 timer.
 */
struct clock64 *STM2_TIM_clock64(void);

#endif /* STM_CLOCK64_H */
