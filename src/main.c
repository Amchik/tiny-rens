#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <curl/curl.h>

#define BUFF_SIZE 1024
#define PORT      "53"
#define HOSTNAME  "127.0.0.51"
#define DNSSERVER "1.1.1.1"

typedef struct {
	uint8_t *ptr;
	size_t   len;
} rens_buffer;
typedef struct {
	uint8_t *ptr;
	size_t   len;
	size_t   cap;
} rens_vec;

size_t curl_readfn(char *ptr, size_t size, size_t nmemb, void *userp) {
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
size_t curl_writefn(void *data, size_t size, size_t nmemb, void *userp) {
	rens_vec *r;
	size_t nread;

	r = (rens_vec*)userp;
	nread = size * nmemb < (r->cap - r->len) ? size * nmemb : (r->cap - r->len);

	memcpy(r->ptr + r->len, data, nread);
	r->len += nread;

	return nread;
}

int main(int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	char buff[BUFF_SIZE], clenbuff[256], *port, *hostname, *dnsserver, url[32];
	rens_buffer rbuff;
	rens_vec    rvec;
	CURL *curl;
	struct curl_slist *headers;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family    = AF_UNSPEC;
	hints.ai_socktype  = SOCK_DGRAM;
	hints.ai_flags     = AI_PASSIVE;
	hints.ai_protocol  = 0;
	hints.ai_canonname = 0;
	hints.ai_addr      = 0;
	hints.ai_next      = 0;

	if (argc > 1 && (!strcmp("-h", argv[1]) || !strcmp("--help", argv[1]))) {
		printf("Usage: %s [port | -] [hostname | -] [dnsserver]\n"
			   "       %s [--help | -h]\n"
			   "Default: port=%s hostname=%s\n"
			   "         dnsserver=%s\n",
			   argv[0], argv[0], PORT, HOSTNAME, DNSSERVER);
		return 0;
	}

	if (argc >= 2 && argv[1][0] != '-')
		port = argv[1];
	else
		port = PORT;
	if (argc >= 3 && argv[2][0] != '-')
		hostname = argv[2];
	else
		hostname = HOSTNAME;
	if (argc >= 4 && argv[3][0] != '-')
		dnsserver = argv[3];
	else
		dnsserver = DNSSERVER;

	s = getaddrinfo(hostname, port, &hints, &result);
	if (s != 0) {
		perror("getaddrinfo()");
		return 1;
	} else {
		printf("Started DNS server on %s:%s\n", hostname, port);
	}

	for (rp = result; rp != 0; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sfd);
	}

	freeaddrinfo(result);

	if (rp == 0) {
		if (errno)
			perror("Could not bind");
		else
			puts("Could not bind");
		return 2;
	}

	snprintf(url, sizeof(url), "https://%s/dns-query", dnsserver);

	for (;;) {
		peer_addr_len = sizeof(peer_addr);
		nread = recvfrom(sfd, buff, sizeof(buff), 0,
				(struct sockaddr *) &peer_addr, &peer_addr_len);
		if (nread == -1)
			continue;

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

		sendto(sfd, rvec.ptr, rvec.len, 0,
				(struct sockaddr*) &peer_addr,
				peer_addr_len);

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}

	return 0;
}
