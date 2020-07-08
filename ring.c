#include <assert.h>
#include <stdlib.h>

#include "ring.h"

bool ring_create(struct event_ring * ring, size_t sz) {
	int ret = pthread_mutex_init(&ring->mutex, NULL);
	assert(!ret);

	ring->data = calloc(sz, sizeof *(ring->data));
	if (!ring->data) {
		int ret = pthread_mutex_destroy(&ring->mutex);
		assert(!ret);
		return false;
	}
	ring->start = ring->end = 0;
	ring->size = sz;
	ring->empty = true;

	return true;
}

void ring_free(struct event_ring * ring) {
	int ret = pthread_mutex_destroy(&ring->mutex);
	assert(!ret);
	free(ring->data);
	ring->start = ring->end = ring->size = 0;
	ring->data = NULL;
	ring->empty = true;
}

void event_write(struct event_ring * ring, unsigned int ev) {
	int err = pthread_mutex_lock(&ring->mutex);
	assert(!err);

	if (ring->end == ring->start) {
		// About to overwrite head of the ring buffer?
		if (ring->empty) { // Not really, we just started.
			ring->empty = false;
		} else {
			// Yes, so move the head marker to point to the next element.
			ring->start += 1;
			ring->start %= ring->size;
		}
	}
	ring->data[ring->end] = ev;
	ring->end += 1;
	ring->end %= ring->size;

	err = pthread_mutex_unlock(&ring->mutex);
	assert(!err);
}

bool event_read(struct event_ring * ring, unsigned int * ev) {
	int err = pthread_mutex_lock(&ring->mutex);
	assert(!err);

	bool ret = false;
	if (!ring->empty) {
		*ev = ring->data[ring->start];
		ret = true;
		ring->start += 1;
		ring->start %= ring->size;
		if (ring->start == ring->end) {
			// We caught up with the end marker.
			ring->empty = true;
		}
	}

	err = pthread_mutex_unlock(&ring->mutex);
	assert(!err);

	return ret;
}
