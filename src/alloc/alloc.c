#include <stdio.h>
#include <sys/mman.h>

#include "../include/alloc.h"

void *alloc(size_t count) {
	size_t *result;
	result = mmap(0, count + sizeof(size_t), PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS, 0, 0);
	*result = count;

	return result + 1;
}

