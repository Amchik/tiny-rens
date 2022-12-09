#include <string.h>

#include "../include/rens-dns.h"

size_t rens_write_question(const struct RensQuestion *question, uint8_t *buff) {
	size_t namelen;

	namelen = question->qname.len;
	memcpy(buff, question->qname.ptr, namelen);

	buff[namelen]     = question->qtype  >> 8;
	buff[namelen + 1] = question->qtype  & 0x00FF;

	buff[namelen + 2] = question->qclass >> 8;
	buff[namelen + 3] = question->qclass & 0x00FF;

	return namelen + 4;
}
