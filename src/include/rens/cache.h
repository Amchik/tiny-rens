/* vim: ft=c
 * Rens (tiny-rens) simple caching
 */

#pragma once

#include <stdint.h>
#include <pthread.h>
#include <time.h>

#include "types.h"
#include "dns.h"
#include "pretty.h"

typedef struct {
	rens_buffer buff;
	time_t created;
} rc_object;
typedef struct {
	rc_object *ptr;
	size_t len;
	size_t cap;

	pthread_mutex_t mut;
} rc_vector;

rc_vector  rc_new();
rc_object *rc_find(const rc_vector *vec, struct RensQuestion q);
rc_object *rc_push(rc_vector *vec, uint8_t *data, size_t len);
void       rc_extend(rc_vector *vec);
void       rc_shink(rc_vector *vec);
