// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "msgpack_abuffer.h"

#include <memory.h>

void msgpack_abuffer_init(msgpack_abuffer_t *abuf, msgpack_abuffer_data_t *data, size_t max_size) {
  abuf->cur_size = 0;
  abuf->data = data;
  abuf->max_size = max_size;
}

int msgpack_abuffer_write(void *data, const msgpack_abuffer_data_t *buf, size_t len) {
  msgpack_abuffer_t *abuf = (msgpack_abuffer_t *)data;
  if (abuf->max_size - abuf->cur_size < len) return -1;
  (void)memcpy(abuf->data + abuf->cur_size, buf, len);
  return 0;
}

void msgpack_abuffer_clear(msgpack_abuffer_t *abuf) { abuf->cur_size = 0; }

size_t msgpack_abuffer_size(const msgpack_abuffer_t *abuf) { return abuf->cur_size; }

const msgpack_abuffer_data_t *msgpack_abuffer_data(const msgpack_abuffer_t *abuf) { return abuf->data; }
