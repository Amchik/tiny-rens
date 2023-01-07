#include <stdio.h>
#include <sys/mman.h>

#include "../include/alloc.h"

void dealloc(void *ptr) {
	size_t *fptr = ptr;
	fptr -= 1;

	munmap(fptr, *fptr);
}

