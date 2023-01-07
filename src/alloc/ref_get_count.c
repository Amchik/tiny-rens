#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

uint8_t ref_get_count(void *ptr) {
	uint8_t *fptr = ptr - 1;

	return *fptr;
}

