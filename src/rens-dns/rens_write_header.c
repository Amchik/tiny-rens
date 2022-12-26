#include "../include/rens.h"

size_t rens_write_header(const struct RensHeader *header, uint8_t *buff) {
	buff[0] = header->id >> 8;
	buff[1] = header->id & 0x00FF;
	buff[2] =
		  ((header->flags & RENS_QR) ? 0x80 : 0)
		+ (header->opcode << 3)
		+ ((header->flags & RENS_AA) ? 0x04 : 0)
		+ ((header->flags & RENS_TC) ? 0x02 : 0)
		+ ((header->flags & RENS_RD) ? 0x01 : 0);
	buff[3] =
		  ((header->flags & RENS_RA) ? 0x80 : 0)
		+ header->rcode;

	buff[4]  = header->qdcount >> 8;
	buff[5]  = header->qdcount & 0x00FF;

	buff[6]  = header->ancount >> 8;
	buff[7]  = header->ancount & 0x00FF;

	buff[8]  = header->nscount >> 8;
	buff[9]  = header->nscount & 0x00FF;

	buff[10] = header->arcount >> 8;
	buff[11] = header->arcount & 0x00FF;

	return 12;
}
