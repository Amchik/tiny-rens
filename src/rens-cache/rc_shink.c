#include <stdint.h>
#include <string.h>

#include "../include/log.h"
#include "../include/rens.h"

/* This part also stolen from rens. It should be
 * inlined, so we add magic __inline.
 * btw idk what this function do.
 */
__inline static uint32_t rc_object_ttl(uint8_t *data) {
	struct RensHeader h;
	struct RensQuestion q;
	struct RensResource r;
	size_t off, ttl, i;

	ttl = 0;

	off = 12;
	h = rens_read_header(data);
	for (i = 0; i < h.qdcount; ++i) {
		q = rens_read_question(data + off);
		off += q.qname.len + 2 * sizeof(uint16_t);
	}
	for (i = 0; i < h.ancount; ++i) {
		r = rens_read_resource(data + off);
		off += r.name.len + r.rdata.len + 10;

		if (ttl == 0 || r.ttl < ttl)
			ttl = r.ttl;
	}

	return ttl;
}


void rc_shink(rc_vector *vec) {
	ssize_t i;
	size_t now, ttl;
	rc_object o;

	now = time(0);

	for (i = 0; i < vec->len; ++i) {
		o = vec->ptr[i];
		ttl = rc_object_ttl(o.buff.ptr);
		if (o.created + ttl < now) {
			assert_debug(ttl > 1,
				"Its not an bug, just check that ttl really zero");
			free(o.buff.ptr); /* sometimes it produce UB
								 rewrite this shit in rust pls */
			if (i + 1 == vec->len) {
				vec->len -= 1;
			} else {
				/* NOTE: this implementation is blazingly slow
				 * when we need move memory >1 times.
				 * As solution we can stole Vec::retain() method from
				 * rust or just combine moves
				 */
				memmove(vec->ptr + i, vec->ptr + i + 1, vec->len - i);
				--i;
			}
		}
	}
}

