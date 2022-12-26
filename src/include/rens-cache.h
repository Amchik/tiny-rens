/* vim: ft=c
 * Rens (tiny-rens) cache system
 */

#pragma once

#include <stdint.h>
#include <pthread.h>
#include <time.h>

#include "rens-types.h"
#include "rens-dns.h"

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

#ifndef RENSC_DEFAULT_CAP
	#define RENSC_DEFAULT_CAP 64
#elif RENSC_DEFAULT_CAP < 2
	#error RENSC_DERENSC_DEFAULT_CAP must be great than 2
#elif RENSC_DEFAULT_CAP < 8
	#warning RENSRENSC_DEFAULT_CAP < 8. It may cause many \
		reallocs and be slow
#endif

rc_vector  rc_new();
rc_object *rc_find(const rc_vector *vec, struct RensQuestion q);
rc_object *rc_push(rc_vector *vec, uint8_t *data, size_t len);
void       rc_extend(rc_vector *vec);
void       rc_shink(rc_vector *vec);

