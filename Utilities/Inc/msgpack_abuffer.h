/*
 * msgpack_abuffer.h
 */

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

void msgpack_abuffer_init(msgpack_abuffer_t *abuf, msgpack_abuffer_data_t *data, size_t max_size);

int msgpack_abuffer_write(void *data, const msgpack_abuffer_data_t *buf, size_t len);

void msgpack_abuffer_clear(msgpack_abuffer_t *abuf);

size_t msgpack_abuffer_size(const msgpack_abuffer_t *abuf);

const msgpack_abuffer_data_t *msgpack_abuffer_data(const msgpack_abuffer_t *abuf);
