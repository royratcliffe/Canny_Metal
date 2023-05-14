// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

/*!
 * \file
 * \brief Message pack auto-buffer.
 */

#include <stddef.h>

/*!
 * \brief Eight-bit data type.
 *
 * This defaults to \c char rather than \c uint8_t for compatibility with the
 * Message Pack interfaces.
 */
typedef char msgpack_abuffer_data_t;

/*!
 * The layout directly copies the \c msgpack_sbuffer layout albeit that
 * the data pointer indirects to static memory rather than dynamic.
 */
struct msgpack_abuffer {
  size_t cur_size;
  msgpack_abuffer_data_t *data;
  size_t max_size;
};

typedef struct msgpack_abuffer msgpack_abuffer_t;

/*!
 * \brief Initialises a buffer for message packing.
 * \param[in] data Mutable buffer. It must remain allocated, e.g. on the stack,
 * while packing.
 *
 * \note There is no release interface by design; the buffer structure and the
 * memory it manages must be either static or stack-allocated.
 */
void msgpack_abuffer_init(msgpack_abuffer_t *abuf, msgpack_abuffer_data_t *data, size_t max_size);

/*!
 * \brief Writes data to buffer.
 * \param[in] data Pointer to \c msgpack_abuffer cast to void.
 * \returns \c 0 for success; \c -1 for failure.
 *
 * Implements the buffer-write interface for a message packer.
 */
int msgpack_abuffer_write(void *data, const msgpack_abuffer_data_t *buf, size_t len);

void msgpack_abuffer_clear(msgpack_abuffer_t *abuf);

/*!
 * \brief Answers the current size of the buffer.
 */
size_t msgpack_abuffer_size(const msgpack_abuffer_t *abuf);

/*!
 * \brief Answers the start of the buffered data.
 */
const msgpack_abuffer_data_t *msgpack_abuffer_data(const msgpack_abuffer_t *abuf);
