/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026, Roy Ratcliffe, Northumberland, United Kingdom
 */

/*!
 * \file clock64.c
 * \brief Implementation of the clock64 module.
 * \details This file implements the functions declared in clock64.h for the
 * clock64 module.
 */
#include <clock64.h>

#include <containerof.h>

static struct clock64 *clock_of_node(struct cons_node *node) {
  /*
   * Always check for NULL before applying a container adjustment to the node
   * member of the clock64 structure, as a NULL node indicates that there is no
   * source clock associated with the clock. If the node is not NULL, it is safe
   * to apply the adjustment and return a pointer to the containing clock64
   * structure using containerof. If the node is NULL, return NULL to indicate
   * that there is no source clock.
   */
  return node != NULL ? containerof(node, struct clock64, node) : NULL;
}

/*
 * Retrieves the source clock associated with a given clock64 instance by
 * accessing the car field of the cons cell contained within the clock64
 * structure. If the car field is NULL, it indicates that there is no source
 * clock associated with the clock; the function will return NULL. This design
 * allows for a flexible association of a clock64 instance with any source
 * clock, as the car field can hold a pointer to any data type.
 */
struct clock64 *clock64_src(struct clock64 *clock) { return clock_of_node(cons_car_node(&clock->node)); }

uint64_t clock64_now(struct clock64 *clock) {
  /*
   * Call the clock's implementation's now function if it exists, otherwise call
   * the source clock's now function if it exists, otherwise return 0. This
   * allows for a flexible clock hierarchy where each clock can have its own
   * implementation or inherit from a source clock.
   */
  const struct clock64_impl *const impl = clock->impl;
  if (impl && impl->now) return impl->now();
  struct clock64 *const src = clock64_src(clock);
  return src ? clock64_now(src) : 0ULL;
}

/*
 * Avoid division by zero by returning 1 if the clock does not have an
 * implementation or a source clock. This ensures that the function behaves
 * predictably even in cases where the clock is not fully initialised or
 * configured.
 *
 * If the clock has its own implementation, use that to get the ticks per
 * microsecond. If it does not have its own implementation but has a source
 * clock, it will use the source clock to get the ticks per microsecond. If
 * neither is available, return 1, indicating that there is no meaningful ticks
 * per microsecond.
 */
uint64_t clock64_ticks_per_us(struct clock64 *clock) {
  const struct clock64_impl *const impl = clock->impl;
  if (impl && impl->ticks_per_us) return impl->ticks_per_us();
  struct clock64 *const src = clock64_src(clock);
  return src ? clock64_ticks_per_us(src) : 1ULL;
}

uint64_t clock64_now_us(struct clock64 *clock) { return clock64_now(clock) / clock64_ticks_per_us(clock); }

uint64_t clock64_now_ms(struct clock64 *clock) { return clock64_now_us(clock) / 1000UL; }

uint64_t clock64_ticks(struct clock64 *clock) { return clock->ticks; }

uint64_t clock64_ticks_us(struct clock64 *clock) { return clock64_ticks(clock) / clock64_ticks_per_us(clock); }

uint64_t clock64_ticks_ms(struct clock64 *clock) { return clock64_ticks_us(clock) / 1000UL; }

void clock64_select(struct clock64 *clock, struct clock64 *src) {
  /*
   * Remove the clock from its current source's list of sub-clocks by
   * traversing the list of sub-clocks.
   *
   * Prepend the clock to the new source's list of sub-clocks by linking the
   * clock to the current head of the source's sub-clock list.
   */
  (void)cons_node(&clock->node, &src->node);
}

void clock64_tick(struct clock64 *clock, void (*tick)(struct clock64 *clock)) { clock->tick = tick; }

void clock64_sync(struct clock64 *clock) {
  /*
   * Latch the current time from the clock and update the clock's ticks if it
   * has changed since the last tick. If the time has changed, call the clock's
   * tick function and then synchronise all sub-clocks (sub-nodes) of the clock.
   * This ensures that all clocks in the hierarchy are synchronised to the same
   * time and that any necessary updates are performed when the time changes. If
   * the time has not changed, there is no need to synchronise the sub-clocks,
   * as they will already be in sync with the current time.
   *
   * A change in the clock's time is detected by comparing the latched time with
   * the clock's previous ticks (initially 0). If they are different, it
   * indicates that the time has changed since the last tick, and the clock's
   * ticks are updated to the new time. The clock's tick function is then called
   * to perform any necessary updates based on the new time. Finally, all
   * sub-clocks of the clock are synchronised by recursively calling
   * clock64_sync on each sub-clock, ensuring that the entire hierarchy of
   * clocks is synchronised to the new time.
   *
   * Conceptually, a synchronisation event emits a tick event if the clock has a
   * tick function defined and the clock's time changes. In this case, the tick
   * function is called whenever the clock's time changes, allowing the clock to
   * perform any necessary updates or actions based on the new time. This design
   * allows for a flexible and extensible clock hierarchy, where each clock can
   * have its own implementation and tick function, while still being
   * synchronised to a common time source.
   */
  uint64_t ticks = clock64_now(clock);
  if (clock->ticks != ticks) {
    clock->ticks = ticks;
    if (clock->tick) clock->tick(clock);
    for (struct cons_node *node = cons_sub_node(&clock->node); node != NULL; node = cons_cdr_node(node))
      clock64_sync(clock_of_node(node));
  }
}
