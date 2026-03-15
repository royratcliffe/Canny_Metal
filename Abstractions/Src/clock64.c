#include <clock64.h>

#include "containerof.h"

static struct clock64 *clock_of_node(struct cons_node *node) { return containerof(node, struct clock64, node); }

struct clock64 *clock64_src(struct clock64 *clock) { return clock_of_node(cons_car_node(&clock->node)); }

uint64_t clock64_ticks(struct clock64 *clock) { return clock->impl->ticks(); }

uint64_t clock64_ticks_per_us(struct clock64 *clock) { return clock->impl->ticks_per_us(); }

uint64_t clock64_us(struct clock64 *clock) { return clock64_ticks(clock) / clock64_ticks_per_us(clock); }

uint64_t clock64_ms(struct clock64 *clock) { return clock64_us(clock) / 1000UL; }

void clock64_setup(struct clock64 *clock, struct clock64 *src) {
  /*
   * Remove the clock from its current source's list of sub-clocks by
   * traversing the list of sub-clocks.
   *
   * Prepend the clock to the new source's list of sub-clocks by linking the
   * clock to the current head of the source's sub-clock list.
   */
  (void)cons_node(&clock->node, &src->node);
}

void clock64_tick(struct clock64 *clock) {
  if (clock->tick) clock->tick(clock);
}

void clock64_sync(struct clock64 *clock) {
  for (struct cons_node *node = cons_sub_node(&clock->node); node != NULL; node = cons_cdr_node(node))
    clock64_sync(clock_of_node(node));
}
