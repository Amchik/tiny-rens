#include <stdlib.h>

#include "../include/log.h"
#include "../include/rens-cache.h"

void rc_extend(rc_vector *vec) {
	vec->ptr = realloc(vec->ptr,
		sizeof(rc_object) * (vec->cap + RENSC_DEFAULT_CAP));
	assert_debug(vec->ptr != 0, "realloc() should return not NULL");

	vec->cap += RENSC_DEFAULT_CAP;
}

