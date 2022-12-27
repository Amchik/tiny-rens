/* vim: ft=c
 * tiny-rens general functions
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <curl/curl.h>

#include "cache.h"
#include "pretty.h"

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

/* Create and run RC cleaning thread
 */
void run_rc_thread(rc_vector *cache);

