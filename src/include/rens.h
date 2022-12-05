/* vim: ft=c
 * rens (tiny-rens) generic functions
 */

#include <stdlib.h>
#include <stdint.h>

#define VERSION "1.1.0"

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

typedef struct {
	uint8_t *ptr;
	size_t   len;
} rens_buffer;

typedef struct {
	uint8_t *ptr;
	size_t   len;
	size_t   cap;
} rens_vec;

/* Inits server and returns server fd
 */
int init_server(const char* hostname, const char* port);

/* Wait and process message
 */
void server_process(int sfd, const char *url);

