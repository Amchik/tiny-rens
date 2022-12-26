#include <string.h>

#include "../include/log.h"
#include "../include/rens-cache.h"

rc_object *rc_push(rc_vector *vec, uint8_t *data, size_t len) {
	if (vec->len == vec->cap)
		rc_extend(vec);
	assert_debug(vec->len < vec->cap,
		"Vector len must be less than vector cap (after extending)");

	vec->ptr[vec->len].buff.ptr = malloc(len);
	vec->ptr[vec->len].buff.len = len;
	vec->ptr[vec->len].created = time(0);

	memcpy(vec->ptr[vec->len].buff.ptr, data, len);
	vec->len += 1;

	return vec->ptr + vec->len - 1;
}

