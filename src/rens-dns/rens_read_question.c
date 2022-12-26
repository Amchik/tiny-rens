#include "../include/rens.h"

struct RensQuestion rens_read_question(const uint8_t *buff) {
	struct RensQuestion q;
	size_t i;

	for (i = 0; buff[i] != 0; )
		if ((uint8_t)buff[i] == 0xC0) {
			i += 1;
			break;
		} else {
			i += buff[i] + 1;
		}
	q.qname.ptr = (unsigned char*)buff;
	q.qname.len = i + 1;

	q.qtype  = (buff[i + 1] << 8) + buff[i + 2];
	q.qclass = (buff[i + 3] << 8) + buff[i + 4];

	return q;
}
