/*
 * msgpack_abuffer.c
 */

#include "msgpack_abuffer.h"

#include <memory.h>

/*!
 * \param[in] data Mutable buffer. It must remain allocated, e.g. on the stack,
 * while packing.
 *
 * \note There is no release interface by design; the buffer structure and the
 * memory it manages must be either static or stack-allocated.
 */
void msgpack_abuffer_init(msgpack_abuffer_t *abuf, msgpack_abuffer_data_t *data, size_t max_size) {
  abuf->cur_size = 0;
  abuf->data = data;
  abuf->max_size = max_size;
}

/*!
 * \param[in] data Pointer to \c msgpack_abuffer cast to void.
 * \returns \c 0 for success; \c -1 for failure.
 *
 * Implements the buffer-write interface for a message packer.
 */
int msgpack_abuffer_write(void *data, const msgpack_abuffer_data_t *buf, size_t len) {
  msgpack_abuffer_t *abuf = (msgpack_abuffer_t *)data;
  if (abuf->max_size - abuf->cur_size < len) return -1;
  (void)memcpy(abuf->data + abuf->cur_size, buf, len);
  return 0;
}

void msgpack_abuffer_clear(msgpack_abuffer_t *abuf) { abuf->cur_size = 0; }

/*!
 * \brief Answers the current size of the buffer.
 */
size_t msgpack_abuffer_size(const msgpack_abuffer_t *abuf) { return abuf->cur_size; }

/*!
 * \brief Answers the start of the buffered data.
 */
const msgpack_abuffer_data_t *msgpack_abuffer_data(const msgpack_abuffer_t *abuf) { return abuf->data; }
