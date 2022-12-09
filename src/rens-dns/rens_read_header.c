#include <stdint.h>

#include "../include/rens-dns.h"

struct RensHeader rens_read_header(const uint8_t *buff) {
	struct RensHeader h;

	h.id     = (buff[0] << 8) + buff[1];
	h.opcode = (0x78 & buff[2]) >> 3;
	h.rcode  = 0x0F & buff[3];
	h.flags  = 
		  ((buff[2] & 0x80) ? RENS_QR : 0)
		+ ((buff[2] & 0x04) ? RENS_AA : 0)
		+ ((buff[2] & 0x02) ? RENS_TC : 0)
		+ ((buff[2] & 0x01) ? RENS_RD : 0)
		+ ((buff[3] & 0x80) ? RENS_RA : 0);
	
	h.qdcount = (buff[4]  << 8) + buff[5];
	h.ancount = (buff[6]  << 8) + buff[7];
	h.nscount = (buff[8]  << 8) + buff[9];
	h.arcount = (buff[10] << 8) + buff[11];

	return h;
}
