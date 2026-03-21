/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file clock64.h
 * \brief Header file for the clock64 module.
 * \details This header file defines the structures and function prototypes for
 * the clock64 module, which provides a 64-bit clock implementation. The clock64
 * module allows for the creation of clocks that can provide the current time in
 * ticks, as well as the number of ticks per microsecond. The clock64 structure
 * includes a cons node for linking clocks in a hierarchy, an optional
 * implementation for providing time functions, and an optional tick function
 * that can be called on time changes. The clock64 module is designed to be
 * flexible and extensible, allowing for the creation of complex clock
 * hierarchies with various time sources and behaviours.
 */
#ifndef CLOCK64_H
#define CLOCK64_H

#include <cons_node.h>

/*
 * for uint64_t
 */
#include <stdint.h>

struct clock64_impl;

/*!
 * \brief A 64-bit clock.
 * \details A clock is a source of time. It fundamentally provides a "now"
 * function that returns the current time in ticks, and a "ticks per
 * microsecond" function that scales time to real-world units.
 *
 * Clocks arrange in a hierarchy, where each clock has a source clock (its
 * super-clock) and zero or more sub-clocks. This design allows for a flexible
 * and extensible clock hierarchy, where each clock can have its own
 * implementation or inherit from a source clock. All clocks, apart from a root
 * clock, live in a tree structure. More than one root clock can exist at once.
 * Typically, a root clock corresponds to a piece of timer hardware.
 *
 * Clocks do not start and stop. They always run. The "now" function of a clock
 * reads the current time from the clock, which is typically derived from the
 * source clock's "now" function, possibly with some adjustments or
 * transformations. This flexibility allows for complex timing relationships
 * between clocks in the system, as each clock can have its own time function
 * that may depend on the time of its source clock.
 */
struct clock64 {
  /*!
   * \brief The cons node representing this clock in the clock hierarchy.
   * \details This node is used to link the clock into the hierarchical
   * structure of clocks, allowing it to have a source clock (super-clock) and
   * zero or more sub-clocks. The \c car field of the cons cell points to the
   * source clock, while the \c cdr field links sibling clocks together in a
   * list of sub-clocks.
   */
  struct cons_node node;

  /*!
   * \brief Pointer to the clock's optional implementation.
   * \details The implementation provides the actual functions for getting the
   * current time and the ticks per microsecond. If the clock does not have its
   * own implementation, it can inherit these functions from its source clock.
   * \note The implementation is optional, allowing for a flexible clock hierarchy
   * where clocks can either have their own implementation or rely on their source
   * clock's implementation.
   * \note The implementation points to \c const storage, indicating that the
   * functions it points to cannot be modified through this pointer, or not
   * without a cast. This design choice emphasises that the implementation is
   * meant to be a read-only configuration for the clock, and any modifications
   * to the implementation should be done through other means, such as defining
   * a new implementation structure and assigning it to the clock.
   */
  const struct clock64_impl *impl;

  /*!
   * \brief The clock's optional tick function.
   * \details The tick function is called whenever the clock's time changes,
   * which is typically triggered by periodic root-clock synchronisation events.
   * The tick function allows the clock to perform any necessary updates or
   * actions based on the new time. This design allows for a flexible and
   * extensible clock hierarchy, where each clock can have its own tick function
   * to respond to time changes, or it can rely on the tick function of its
   * source clock if it does not have its own.
   * \note The tick function is optional, allowing for clocks that do not need
   * to perform any specific actions on time changes to simply not define a tick
   * function. If a clock does not have its own tick function, it can still be
   * synchronised and will still update its ticks based on the time changes of
   * its source clock, if it has one.
   */
  void (*tick)(struct clock64 *clock);

  /*!
   * \brief The latched time from the clock at the last synchronisation.
   * \details The ticks represent the time that was latched from the clock during
   * the last synchronisation event.
   */
  uint64_t ticks;
};

/*!
 * \brief Implementation of a clock64.
 * \details The implementation provides the actual functions for getting the
 * current time and the ticks per microsecond. Clocks without their own
 * implementation inherit these functions from their source clock.
 */
struct clock64_impl {
  uint64_t (*now)(void);          /*!< Function to latch the current time. */
  uint64_t (*ticks_per_us)(void); /*!< Function to access the number of ticks per microsecond. */
};

/*!
 * \brief Accessor for the source clock of a clock64 instance.
 * \param clock The clock64 instance to access.
 * \retval A pointer to the source clock of the given clock64 instance.
 * \retval NULL if the clock64 instance does not have a source clock.
 */
struct clock64 *clock64_src(struct clock64 *clock);

/*!
 * \brief Latches the "current" time from the clock.
 * \param clock The clock64 instance to read the time from.
 * \retval The current time as a 64-bit unsigned integer.
 * \note If the clock has its own implementation, it will use that to get the
 * current time. If it does not have its own implementation but has a source
 * clock, it will use the source clock to get the current time. If neither is
 * available, it will return 0, indicating that the current time cannot be
 * determined.
 */
uint64_t clock64_now(struct clock64 *clock);

/*!
 * \brief Accessor for the number of ticks per microsecond of a clock64 instance.
 * \param clock The clock64 instance to access.
 * \retval The number of ticks per microsecond.
 */
uint64_t clock64_ticks_per_us(struct clock64 *clock);

/*!
 * \brief Accessor for the current time in microseconds of a clock64 instance.
 * \param clock The clock64 instance to access.
 * \retval The current time in microseconds.
 * \note Rounds down to the nearest microsecond. This is because the current
 * time in ticks is divided by the number of ticks per microsecond using 64-bit
 * integer arithmetic.
 */
uint64_t clock64_now_us(struct clock64 *clock);

/*!
 * \brief Accessor for the current time in milliseconds of a clock64 instance.
 * \param clock The clock64 instance to access.
 * \retval The current time in milliseconds.
 */
uint64_t clock64_now_ms(struct clock64 *clock);

/*!
 * \brief Answers the ticks of the clock at the last synchronisation.
 * \param clock The clock to query.
 * \retval The number of ticks at the last synchronisation.
 * \note The ticks are updated during synchronisation, which is typically
 * triggered by periodic root-clock synchronisation events. The ticks represent
 * the latched time from the clock at the last synchronisation, and they are
 * used to determine if the clock's time has changed since the last tick.
 */
uint64_t clock64_ticks(struct clock64 *clock);

/*!
 * \brief Answers the ticks of the clock in microseconds at the last synchronisation.
 * \param clock The clock to query.
 * \retval The number of ticks in microseconds at the last synchronisation.
 */
uint64_t clock64_ticks_us(struct clock64 *clock);

/*!
 * \brief Answers the ticks of the clock in milliseconds at the last synchronisation.
 * \param clock The clock to query.
 * \retval The number of ticks in milliseconds at the last synchronisation.
 */
uint64_t clock64_ticks_ms(struct clock64 *clock);

/*!
 * \brief Selects the source clock for a clock64 instance.
 * \param clock The clock64 instance to configure.
 * \param src The new source clock to associate with the clock64
 * instance, or NULL to just remove the current source clock.
 * \details This function sets up the source clock for a given clock64 instance by
 * modifying the linked list of sub-clocks associated with the source clock. If
 * the clock64 instance already has a source clock, it is first removed from the
 * list of sub-clocks of the current source clock. Then, if a new source clock
 * is provided, the clock64 instance is added to the list of sub-clocks of the
 * new source clock.
 */
void clock64_select(struct clock64 *clock, struct clock64 *src);

/*!
 * \brief Sets the tick function for a clock64 instance.
 * \param clock The clock64 instance to configure.
 * \param tick The tick function to associate with the clock64 instance.
 */
void clock64_tick(struct clock64 *clock, void (*tick)(struct clock64 *clock));

/*!
 * \brief Recursively synchronises the clock and its sub-clocks.
 * \param clock The clock to synchronise.
 * \details Synchronising means giving compute time to the clock and its
 * sub-clocks, allowing them to update any internal state and perform
 * any clock-related operations. Recursively synchronises all sub-clocks
 * (sub-nodes) of the clock. This ensures that the entire hierarchy of
 * clocks rooted at this clock is synchronised.
 */
void clock64_sync(struct clock64 *clock);

#endif /* CLOCK64_H */
