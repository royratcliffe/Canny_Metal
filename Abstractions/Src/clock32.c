#include <clock32.h>

#include "containerof.h"

static struct clock32 *clock_of_node(struct cons_node *node) { return containerof(node, struct clock32, node); }

struct clock32 *clock32_src(struct clock32 *clock) { return clock_of_node(cons_car_node(&clock->node)); }

uint32_t clock32_ticks(struct clock32 *clock) { return clock->impl->ticks(); }

uint32_t clock32_ticks_per_ms(struct clock32 *clock) { return clock->impl->ticks_per_ms(); }

uint32_t clock32_ms(struct clock32 *clock) { return clock32_ticks(clock) / clock32_ticks_per_ms(clock); }

uint32_t clock32_s(struct clock32 *clock) { return clock32_ms(clock) / 1000UL; }

void clock32_setup(struct clock32 *clock, struct clock32 *src) {
  (void)cons_node(&clock->node, &src->node);
}

void clock32_sync(struct clock32 *clock) {
  if (clock->sync) clock->sync(clock);
  for (struct cons_node *node = cons_sub_node(&clock->node); node != NULL; node = cons_cdr_node(node))
    clock32_sync(clock_of_node(node));
}
