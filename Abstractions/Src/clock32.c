#include <clock32.h>

#include "containerof.h"

static struct clock32 *clock_of_node(struct cons_node *node) { return containerof(node, struct clock32, node); }

struct clock32 *clock32_src(struct clock32 *clock) { return clock_of_node(cons_car_node(&clock->node)); }

uint32_t clock32_ticks(struct clock32 *clock) { return clock->impl->ticks(); }

uint32_t clock32_ticks_per_ms(struct clock32 *clock) { return clock->impl->ticks_per_ms(); }

uint32_t clock32_ms(struct clock32 *clock) { return clock32_ticks(clock) / clock32_ticks_per_ms(clock); }

uint32_t clock32_s(struct clock32 *clock) { return clock32_ms(clock) / 1000UL; }

/*!
 * \brief Get the current time in milliseconds as a floating-point value.
 * \param clock The clock to get the time from.
 * \return The current time in milliseconds as a floating-point value.
 * \note Returns the time in milliseconds as a floating-point value, which
 * allows for fractional milliseconds. The result is calculated by dividing the
 * number of ticks by the number of ticks per millisecond, both of which are
 * obtained from the clock's implementation both separately converted to float
 * to ensure floating-point division.
 */
float clock32_millis(struct clock32 *clock) { return (float)clock32_ticks(clock) / (float)clock32_ticks_per_ms(clock); }

void clock32_setup(struct clock32 *clock, struct clock32 *src) {
  (void)cons_node(&clock->node, &src->node);
}

void clock32_sync(struct clock32 *clock) {
  if (clock->sync) clock->sync(clock);
  for (struct cons_node *node = cons_sub_node(&clock->node); node != NULL; node = cons_cdr_node(node))
    clock32_sync(clock_of_node(node));
}
