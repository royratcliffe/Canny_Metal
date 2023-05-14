// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#pragma once

#include <msgpack.h>

#include "msgpack_abuffer.h"

/*!
 * \brief Message packs to a buffer space by yielding.
 * \param[out] data Start of buffer space.
 * \param[in] size Length of buffer space: the packing limit less than \c INT_MAX.
 * \param[in] yield Packing function answering non-zero for success; zero for failure.
 * \returns Number of bytes packed between \c 0 and \c size else \c -1 if packing fails.
 */
int msgpack_abuffer_yield(void *data, size_t size, int (*yield)(msgpack_packer *packer));

int msgpack_abuffer_opaque_yield(void *data, size_t size, void *opaque,
                                 int (*yield)(void *opaque, msgpack_packer *packer));
