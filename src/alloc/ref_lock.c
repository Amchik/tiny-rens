#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/alloc.h"

void ref_lock(void *ptr) {
	pthread_mutex_lock(ptr - sizeof(uint8_t) - sizeof(pthread_mutex_t));
}

void ref_unlock(void *ptr) {
	pthread_mutex_unlock(ptr - sizeof(uint8_t) - sizeof(pthread_mutex_t));
}

