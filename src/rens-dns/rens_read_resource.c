#include "../include/rens.h"

struct RensResource rens_read_resource(const uint8_t *buff) {
	struct RensResource r;
	size_t i;

	for (i = 0; buff[i] != 0; )
		if ((uint8_t)buff[i] == 0xC0) {
			i += 1;
			break;
		} else {
			i += buff[i] + 1;
		}
	r.name.ptr = (unsigned char*)buff;
	r.name.len = i + 1;

	i++;
	r.qtype  = (buff[i]     << 8) + buff[i + 1];
	r.qclass = (buff[i + 2] << 8) + buff[i + 3];
	r.ttl    =
		  (buff[i + 4] << 24)
		| (buff[i + 5] << 16)
		| (buff[i + 6] << 8)
		| buff[i + 7];
	r.rdata.len = (buff[i + 8] << 8) + buff[i + 9];
	r.rdata.ptr = (unsigned char*)(buff + i + 10);

	return r;
}
