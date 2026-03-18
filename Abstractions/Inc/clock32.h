#include <cons_node.h>

/*
 * for uint32_t
 */
#include <stdint.h>

struct clock32_impl;

/*!
 * \note 32-bit clocks count time in the millisecond range, so they are not
 * suitable for measuring very short durations, such as the time taken to
 * execute a single instruction. For measuring very short durations, a
 * high-resolution 64-bit clock should be used instead.
 * \note 32-bit clocks will wrap around after approximately 49.7 days (assuming
 * one millisecond per tick) so they are not suitable for measuring long
 * durations. They are typically used for measuring short durations, such as the
 * time taken to execute a function or the time between two events. For
 * measuring long durations, a 64-bit clock should be used instead.
 */
struct clock32 {
  /*!
   * \brief The cons node representing this clock in the clock hierarchy.
   * \details This node links the clock into the hierarchical structure of
   * clocks, enabling a source clock (super-clock) and zero or more sub-clocks.
   * The \c car field points to the source clock, while the \c cdr field links
   * sibling clocks together in a list of sub-clocks.
   */
  struct cons_node node;

  /*!
   * \brief Pointer to the clock's optional implementation.
   * \details The implementation provides the actual functions for getting the
   * current time and the ticks per millisecond. Clocks without their own
   * implementation inherit these functions from their source clock.
   * \note The implementation is optional, allowing for a flexible clock hierarchy
   * where clocks can either have their own implementation or rely on their source
   * clock's implementation.
   * \note The \c const storage prevents modifications through this pointer
   * without a cast. This emphasizes that the implementation is a read-only
   * configuration for the clock. Modifications require defining a new
   * implementation structure and assigning it to the clock.
   */
  const struct clock32_impl *impl;

  /*!
   * Call it a clock "pulse" function.
   */
  void (*tick)(struct clock32 *clock);
};

struct clock32_impl {
  uint32_t (*ticks)(void);
  uint32_t (*ticks_per_ms)(void);
};

uint32_t clock32_ticks(struct clock32 *clock);

uint32_t clock32_ticks_per_ms(struct clock32 *clock);

/*!
 * \brief Get the current time in milliseconds.
 * \param clock The clock to get the time from.
 * \return The current time in milliseconds.
 * \note Does \e not round up to the nearest millisecond, so the result may be
 * slightly less than the actual time in milliseconds. Returns the number of
 * whole milliseconds that have elapsed since the clock was started.
 */
uint32_t clock32_ms(struct clock32 *clock);

uint32_t clock32_s(struct clock32 *clock);

float clock32_millis(struct clock32 *clock);

/*!
 * \brief Synchronise the clock and its child clocks.
 * \param clock The clock to synchronise.
 * \details Synchronising means giving compute time to the clock and its
 * child clocks, allowing them to update any internal state and perform
 * any clock-related operations. This function calls the \c tick
 * function of this clock, if defined, and then recursively synchronises
 * all child clocks (sub-nodes) of the clock. This ensures that the
 * entire hierarchy of clocks is synchronised.
 */
void clock32_sync(struct clock32 *clock);
