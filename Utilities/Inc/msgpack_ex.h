/*
 * msgpack_ex.h
 */

#pragma once

#include <msgpack.h>

#include <string.h>

static inline int msgpack_pack_str_of_len(msgpack_packer *packer, const char *str) {
  return msgpack_pack_str_with_body(packer, str, strlen(str));
}
