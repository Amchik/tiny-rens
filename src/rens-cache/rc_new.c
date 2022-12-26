#include <stdlib.h>

#include "../include/rens-cache.h"

rc_vector rc_new() {
	rc_vector r;

	r.ptr = malloc(RENSC_DEFAULT_CAP * sizeof(rc_object));
	r.cap = RENSC_DEFAULT_CAP;
	r.len = 0;

	pthread_mutex_init(&r.mut, 0);

	return r;
}

