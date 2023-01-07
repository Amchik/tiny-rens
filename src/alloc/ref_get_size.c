#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

size_t ref_get_size(void *ptr) {
	uint8_t *fptr = ptr;
	fptr -= sizeof(uint8_t) + sizeof(pthread_mutex_t);

	return mem_getsize(fptr) - sizeof(uint8_t) - sizeof(pthread_mutex_t);
}

