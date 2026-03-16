#include <clock64.h>

#include "containerof.h"

static struct clock64 *clock_of_node(struct cons_node *node) {
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

uint64_t clock64_ticks_per_us(struct clock64 *clock) {
  const struct clock64_impl *const impl = clock->impl;
  if (impl && impl->ticks_per_us) return impl->ticks_per_us();
  struct clock64 *const src = clock64_src(clock);
  return src ? clock64_ticks_per_us(src) : 0ULL;
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
   */
  uint64_t ticks = clock64_now(clock);
  if (clock->ticks != ticks) {
    clock->ticks = ticks;
    if (clock->tick) clock->tick(clock);
    for (struct cons_node *node = cons_sub_node(&clock->node); node != NULL; node = cons_cdr_node(node))
      clock64_sync(clock_of_node(node));
  }
}
