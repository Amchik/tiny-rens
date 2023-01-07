#include <stdio.h>
#include <sys/mman.h>

#include "../include/alloc.h"

size_t mem_getsize(void *ptr) {
	size_t *fptr = ptr;
	fptr -= 1;

	return *fptr - sizeof(size_t);
}

