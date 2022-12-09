#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

#include "../include/rens-dns.h"
#include "../include/log.h"

static size_t curl_readfn(char *ptr, size_t size, size_t nmemb, void *userp);
static size_t curl_writefn(void *data, size_t size, size_t nmemb, void *userp);

__inline static void log_question(uint8_t *buff);
__inline static void log_answer(uint8_t *buff);

void server_process(int sfd, const char *url) {
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	uint8_t buff[BUFF_SIZE];
	char clenbuff[128];
	rens_buffer rbuff;
	rens_vec    rvec;
	CURL *curl;
	struct curl_slist *headers;

	peer_addr_len = sizeof(peer_addr);
	nread = recvfrom(sfd, buff, sizeof(buff), 0,
			(struct sockaddr *) &peer_addr, &peer_addr_len);
	if (nread < 2)
		return;

	log_question(buff);

	curl = curl_easy_init();

	rbuff.ptr = (unsigned char*)buff;
	rbuff.len = nread;

	rvec.ptr = (unsigned char*)buff;
	rvec.len = 0;
	rvec.cap = sizeof(buff);

	sprintf(clenbuff, "Content-Length: %lu", rbuff.len);

	headers = curl_slist_append(0, "Content-Type: application/dns-message");
	headers = curl_slist_append(headers, clenbuff);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_readfn);
	curl_easy_setopt(curl, CURLOPT_READDATA,     &rbuff);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefn);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &rvec);

	curl_easy_perform(curl);

	log_answer(buff);

	sendto(sfd, rvec.ptr, rvec.len, 0,
			(struct sockaddr*) &peer_addr,
			peer_addr_len);

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
}

static size_t curl_readfn(char *ptr, size_t size, size_t nmemb, void *userp) {
	rens_buffer *r;
	size_t nread;

	r = (rens_buffer*)userp;
	nread = size * nmemb < r->len ? size * nmemb : r->len;

	if (r->len == 0)
		return 0;

	memcpy(ptr, r->ptr, nread);
	r->len -= nread;
	r->ptr += nread;

	return nread;
}
static size_t curl_writefn(void *data, size_t size, size_t nmemb, void *userp) {
	rens_vec *r;
	size_t nread;

	r = (rens_vec*)userp;
	nread = size * nmemb < (r->cap - r->len) ? size * nmemb : (r->cap - r->len);

	memcpy(r->ptr + r->len, data, nread);
	r->len += nread;

	return nread;
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
