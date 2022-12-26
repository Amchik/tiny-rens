/* vim: ft=c
 * Rens (tiny-rens) primitive types declaration
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "pretty.h"

typedef struct {
	uint8_t *ptr;
	size_t   len;
	size_t   cap;
} rens_vec;

typedef struct {
	uint8_t *ptr;
	size_t   len;
} rens_buffer;

