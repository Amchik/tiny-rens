#include <errno.h>
#include <string.h>
#include <curl/curl.h>
#include <stdint.h>

#include "../include/rens.h"

size_t perform_request(CURL *curl, uint8_t *data, size_t n) {
	rens_buffer rbuff;
	rens_vec    rvec;
	char content_length[256];
	struct curl_slist *headers;

	rbuff.ptr = data;
	rbuff.len = n;

	rvec.ptr = data;
	rvec.len = 0;
	rvec.cap = BUFF_SIZE;

	sprintf(content_length, "Content-Length: %lu", n);

	headers = curl_slist_append(0, "Content-Type: application/dns-message");
	headers = curl_slist_append(headers, content_length);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_READDATA, &rbuff);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rvec);

	curl_easy_perform(curl);
	curl_slist_free_all(headers);

	return rvec.len;
}
