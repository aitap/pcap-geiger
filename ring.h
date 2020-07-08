#ifndef RING_H
#define RING_H
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

struct event_ring {
	unsigned int * data;
	size_t size, start, end;
	pthread_mutex_t mutex;
	bool empty;
};

bool ring_create(struct event_ring *, size_t);
void ring_free(struct event_ring *);

void event_write(struct event_ring *, unsigned int);
bool event_read(struct event_ring *, unsigned int *);
#endif
