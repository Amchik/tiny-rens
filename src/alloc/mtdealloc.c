#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

void mtdealloc(void *ptr) {
	uint8_t *fptr = ptr;
	fptr -= 1;
	/* assert_debug(*fptr < 2); */
	fptr -= sizeof(pthread_mutex_t);
	/* should we destroy mutex? */

	dealloc(fptr);
}

