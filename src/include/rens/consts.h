/* vim: ft=c
 * tiny-rens (Rens) constants
 */

#pragma once

#include "pretty.h"

#define BASE_VERSION "1.3.0"

#ifndef BUILD_RELEASE
	#ifndef BUILD_COMMIT
		#define BUILD_COMMIT "dev (" __DATE__ " " __TIME__ ")"
	#endif
	#define VERSION BASE_VERSION "-" BUILD_COMMIT
#else
	#define VERSION BASE_VERSION
#endif

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

#ifndef RENSC_DEFAULT_CAP
	#define RENSC_DEFAULT_CAP 64
#elif RENSC_DEFAULT_CAP < 2
	#error RENSC_DERENSC_DEFAULT_CAP must be great than 2
#elif RENSC_DEFAULT_CAP < 8
	#warning RENSRENSC_DEFAULT_CAP < 8. It may cause many reallocs and be slow
#endif

