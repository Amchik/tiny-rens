/* vim: ft=c
 * TODO: FIX THAT SHIT
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint8_t *ptr;
	size_t   len;
	size_t   cap;
} rens_vec;

typedef struct {
	uint8_t *ptr;
	size_t   len;
} rens_buffer;

