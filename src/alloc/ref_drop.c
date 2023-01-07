#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

uint8_t ref_drop(void *ptr) {
	uint8_t r;
	ref_lock(ptr);
	if ((r = ref_drop_unlocked(ptr)))
		ref_unlock(ptr);
	return r;
}

uint8_t ref_drop_unlocked(void *ptr) {
	uint8_t *fptr = ptr;
	fptr -= 1;
	*fptr -= 1;
	if (*fptr == 0) {
		mtdealloc(ptr);
		return 0;
	}
	return *fptr;
}

