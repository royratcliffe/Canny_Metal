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

  const struct clock64_impl *impl;

  void (*tick)(struct clock64 *clock);

  uint64_t ticks;
};

struct clock64_impl {
  uint64_t (*now)(void);

  uint64_t (*ticks_per_us)(void);
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

uint64_t clock64_ticks_per_us(struct clock64 *clock);

uint64_t clock64_now_us(struct clock64 *clock);

uint64_t clock64_now_ms(struct clock64 *clock);

uint64_t clock64_ticks(struct clock64 *clock);

uint64_t clock64_ticks_us(struct clock64 *clock);

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
