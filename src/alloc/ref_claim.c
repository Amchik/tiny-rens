#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

void ref_claim(void *ptr) {
	ref_lock(ptr);
	ref_claim_unlocked(ptr);
	ref_unlock(ptr);
}

void ref_claim_unlocked(void *ptr) {
	uint8_t *fptr = ptr;
	fptr -= 1;
	*fptr += 1;
}

