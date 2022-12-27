#include <stdio.h>
#include <string.h>

#include "include/rens.h"
#include "include/log.h"

int LOG_LEVEL = LWARN;
enum { CP_NONE, CP_NORMAL, CP_NOTTL } CACHE_POLICY = CP_NORMAL;

static char *arguments[3] = { D_PORT, D_HOSTNAME, D_DNSSERVER };

void parse_argv(int argc, char **argv);

int main(int argc, char **argv) {
	char url[64];
	int sfd;
	CURL *curl;
	rc_vector cache, *cache_ptr;

	parse_argv(argc, argv);
	sfd = init_server(arguments[1], arguments[0]);

	if (sfd < 0)
		return -sfd;

	logf(LINFO, "Started DNS server on %s:%s", arguments[1], arguments[0]);

	snprintf(url, sizeof(url), "https://%s/dns-query", arguments[2]);
	logf(LINFO, "Using DNS server: %s, %s", arguments[2], url);

	curl = create_curl_instance(url);
	if (CACHE_POLICY != CP_NONE)
		cache = rc_new();

	if (CACHE_POLICY == CP_NORMAL)
		run_rc_thread(&cache);

	cache_ptr = CP_NONE ? 0 : &cache;
	for (;;) {
		server_process(sfd, curl, cache_ptr);
	}

	return 0;
}

void parse_argv(int argc, char **argv) {
	int n;
	size_t classic_cursor;

	classic_cursor = 0;

	for (n = 1; n < argc; ++n) {
		if (argv[n][0] == '-') {
			if (argv[n][1] == 0) {
				classic_cursor++;
			} else {
				int r;
				for (r = 1; argv[n][r] != 0; ++r)
					switch (argv[n][r]) {
						case 'Q':
							LOG_LEVEL = LPRIM;
							break;
						case 'q':
							LOG_LEVEL = LERR;
							break;
						case 'v':
							LOG_LEVEL = LINFO;
							break;
						case 'g':
							LOG_LEVEL = LDEBUG;
							break;
						case 'c':
							CACHE_POLICY = CP_NONE;
							break;
						case 'C':
							CACHE_POLICY = CP_NOTTL;
							break;
						case 'h':
							printf(
									"Usage: %s [-gvqQhcC] [port] [hostname] [dns_server]\n"
									"port, hostname and dns_server can be '-'\n"
									"\n"
									"[c] no cache [C] cache wo/ ttl\n"
									"[g] debug [v] info [q] errors [Q] nothing [h] help\n"
									"tiny-rens v" VERSION " made by ValgrindLLVM with <3\n",
									argv[0]);
							exit(0);
							break;
					}
			}
		} else if (classic_cursor < sizeof(arguments)) {
			arguments[classic_cursor] = argv[n];
			classic_cursor++;
		}
	}
}

