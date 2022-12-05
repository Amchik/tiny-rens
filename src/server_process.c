#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>

#include "include/rens.h"
#include "include/log.h"

static size_t curl_readfn(char *ptr, size_t size, size_t nmemb, void *userp);
static size_t curl_writefn(void *data, size_t size, size_t nmemb, void *userp);

void server_process(int sfd, const char *url) {
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	char buff[BUFF_SIZE], clenbuff[128];
	rens_buffer rbuff;
	rens_vec    rvec;
	CURL *curl;
	struct curl_slist *headers;

	peer_addr_len = sizeof(peer_addr);
	nread = recvfrom(sfd, buff, sizeof(buff), 0,
			(struct sockaddr *) &peer_addr, &peer_addr_len);
	if (nread < 2)
		return;

	dlogf("%d Recieved DNS query of %lu bytes",
			(buff[0] << 8) + buff[1], nread);

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

	dlogf("%d Sending DNS answer of length %lu bytes",
			(buff[0] << 8) + buff[1], rvec.len);

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
