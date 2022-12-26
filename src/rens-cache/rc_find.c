#include "../include/rens.h"

/* By the way, this function every time parses
 * vector question block and trying to find q.qname == el.qname.
 * It may be slow... Also, it assumes that there only one
 * question per request.
 * This part stolen from rens
 */
rc_object *rc_find(const rc_vector *vec, struct RensQuestion q) {
	rc_object *o;
	size_t i, j;
	struct RensQuestion el;

	for (i = 0; i < vec->len; ++i) {
		o = vec->ptr + i;
		el = rens_read_question(o->buff.ptr + 12);
		if (q.qclass != el.qclass
				|| q.qtype != el.qtype
				|| q.qname.len != el.qname.len)
			continue;
		for (j = 0; j < q.qname.len; ++j)
			if (q.qname.ptr[j] != el.qname.ptr[j])
				continue;
		return o;
	}


	return 0;
}

