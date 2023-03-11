/*!
 * \file container_of.h
 */

#pragma once

#include <stddef.h>

/*!
 * \brief Casts member pointer to containing structure pointer.
 * \param ptr Pointer to member.
 * \param type Type of container structure.
 * \param member Name of container's member.
 *
 * Utilises GCC's `offsetof` built-in function to displace the member pointer
 * backwards by the difference between the container's address and the member's
 * address. Subtracting a size from a void pointer decrements the pointer by one
 * byte for each displacement unit.
 */
#define CONTAINER_OF(ptr, type, member) ({ \
	((type *)((void *)(ptr) - offsetof(type, member))); \
})
