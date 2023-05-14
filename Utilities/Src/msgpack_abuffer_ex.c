// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "msgpack_abuffer_ex.h"

int msgpack_abuffer_yield(void *data, size_t size, int (*yield)(msgpack_packer *packer)) {
  msgpack_abuffer_t buffer;
  msgpack_packer packer;
  msgpack_abuffer_init(&buffer, data, size);
  msgpack_packer_init(&packer, &buffer, msgpack_abuffer_write);
  return yield(&packer) ? msgpack_abuffer_size(&buffer) : -1;
}

int msgpack_abuffer_opaque_yield(void *data, size_t size, void *opaque,
                                 int (*yield)(void *opaque, msgpack_packer *packer)) {
  msgpack_abuffer_t buffer;
  msgpack_packer packer;
  msgpack_abuffer_init(&buffer, data, size);
  msgpack_packer_init(&packer, &buffer, msgpack_abuffer_write);
  return yield(opaque, &packer) ? msgpack_abuffer_size(&buffer) : -1;
}
