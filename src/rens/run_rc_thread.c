#include <pthread.h>
#include <unistd.h>

#include "../include/rens.h"

void *rc_thread_loop(void *_userp) {
	rc_vector *cache;

	cache = (rc_vector*)_userp;

	for (;;) {
		sleep(3);
		pthread_mutex_lock(&cache->mut);
		rc_shink(cache);
		pthread_mutex_unlock(&cache->mut);
	}

	return 0;
}

void run_rc_thread(rc_vector *cache) {
	pthread_t th;

	pthread_create(&th, 0, rc_thread_loop, cache);
}
