#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

const pthread_mutex_t *ref_get_mutex(void *ptr) {
	uint8_t *fptr = ptr;
	fptr -= sizeof(uint8_t) + sizeof(pthread_mutex_t);

	return (void*)fptr;
}

