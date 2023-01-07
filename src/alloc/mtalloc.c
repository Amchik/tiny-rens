#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

void *mtalloc(size_t count) {
	/* i wish alloc() function will be inlined */
	uint8_t *ptr;
	pthread_mutex_t *mut;

	ptr = alloc(count + sizeof(pthread_mutex_t) + sizeof(uint8_t));
	mut = (void*)ptr;
	pthread_mutex_init(mut, 0);
	ptr += sizeof(pthread_mutex_t);
	*ptr = 1;

	return ptr + 1;
}

