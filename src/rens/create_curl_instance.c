#include <string.h>
#include <curl/curl.h>

#include "../include/rens.h"

static size_t curl_readfn(char *ptr, size_t size, size_t nmemb, void *userp);
static size_t curl_writefn(void *data, size_t size, size_t nmemb, void *userp);

CURL *create_curl_instance(const char *url) {
	CURL *curl;
	
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_readfn);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefn);

	return curl;
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
