/* vim: ft=c
 * rens (tiny-rens) generic functions
 */

#pragma once

#include "rens-cache.h"
#include <stdlib.h>
#include <stdint.h>
#include <curl/curl.h>

#define VERSION "1.2.0"

#define D_PORT      "53"
#define D_HOSTNAME  "127.0.0.51"
#define D_DNSSERVER "1.1.1.1"

#ifdef BUFF_SIZE
	#if BUFF_SIZE < 512
		#ifdef BUFF_SIZE_NO_ERROR
			#warning RFC1035 requires 512 bytes for messages (BUFF_SIZE < 512)
		#else
			#error RFC1035 requires 512 bytes for messages (BUFF_SIZE < 512)
		#endif
	#endif
#else
	#define BUFF_SIZE 1024
#endif

/* Inits server and returns server fd
 */
int init_server(const char* hostname, const char* port);

/* Create cURL instance
 */
CURL *create_curl_instance(const char *url);

/* Wait and process message
 */
void server_process(int sfd, CURL *url, rc_vector *cache);

/* Perform DoH request
 */
size_t perform_request(CURL *curl, uint8_t *data, size_t n);

