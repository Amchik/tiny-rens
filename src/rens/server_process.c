#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

#include "../include/rens-dns.h"
#include "../include/log.h"

__inline static void log_question(uint8_t *buff);
__inline static void log_answer(uint8_t *buff);

void server_process(int sfd, CURL *curl) {
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	uint8_t buff[BUFF_SIZE];

	peer_addr_len = sizeof(peer_addr);
	nread = recvfrom(sfd, buff, sizeof(buff), 0,
			(struct sockaddr *) &peer_addr, &peer_addr_len);
	if (nread < 2)
		return;

	log_question(buff);

	nread = perform_request(curl, buff, nread);

	log_answer(buff);

	sendto(sfd, buff, nread, 0,
			(struct sockaddr*) &peer_addr,
			peer_addr_len);
}

#if !defined(NO_DEBUG_LOG)
__inline static void log_question(uint8_t *buff) {
	size_t i, j, k, m, off;
	struct RensHeader h;

	h = rens_read_header(buff);
	dlogf("%lu Recieved query ID %u", h.id, h.id);
	dlogf("%lu  QDCOUNT: %u", h.id, h.qdcount);

	off = 12;
	for (i = 0; i < h.qdcount; ++i) {
		struct RensQuestion q;
		uint8_t qname[256];

		q = rens_read_question(buff + off);
		off += q.qname.len + 4;

		m = rens_uncompress_message(buff, q.qname.ptr - buff, qname, sizeof(qname));
		for (j = 0; j < m; ) {
			k = qname[j];
			if (qname[j] == 0)
				qname[j + 1] = 0;
			qname[j] = '.';
			j += k + 1;
		}

		dlogf("%lu >> QUESTION: %u %u <%s>", h.id, q.qtype, q.qclass, qname);
	}
}

__inline static void log_answer(uint8_t *buff) {
	size_t j, k, m, off;
	struct RensHeader h;
	struct RensQuestion q;
	struct RensResource r;
	uint8_t qname[256];

	h = rens_read_header(buff);
	dlogf("%u  RCODE:   %u", h.id, h.rcode);
	dlogf("%u  ANCOUNT: %u", h.id, h.ancount);

	off = 12;
	for (size_t i = 0; i < h.qdcount; ++i) {
		q = rens_read_question(buff + off);
		off += q.qname.len + 4;
	}
	for (size_t i = 0; i < h.ancount; ++i) {
		r = rens_read_resource(buff + off);
		off += r.rdata.len + r.name.len + 10;

		m = rens_uncompress_message(buff, r.name.ptr - buff, qname, sizeof(qname));
		for (j = 0; j < m; ) {
			k = qname[j];
			if (qname[j] == 0)
				qname[j + 1] = 0;
			qname[j] = '.';
			j += k + 1;
		}

		dlogf("%lu >> ANSWER: <%s> %u %u ttl=%u", h.id, qname, r.qtype, r.qclass, r.ttl);
	}
	dlogf("");
}
#else
__inline static void log_question(uint8_t *buff) {}
__inline static void log_answer(uint8_t *buff) {}
#endif
