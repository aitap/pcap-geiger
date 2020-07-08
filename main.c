#include <pthread.h>
#include <stdio.h>

#include "cap.h"
#include "snd.h"

#ifndef RING_SIZE
	#define RING_SIZE 1024
#endif

int main(int argc, char ** argv) {
	if (argc != 3) {
		printf("Usage: %s <pcap source> <pcap filter>\n", argv[0]);
		return 1;
	}

	struct event_ring ring;
	if (!ring_create(&ring, RING_SIZE)) return -2;

	struct cap_ctx cap_arg = {
		.source = argv[1],
		.filter = argv[2],
		.ring = &ring
	};
	pthread_t cap_thread;
	if (pthread_create(&cap_thread, NULL, &cap_thread_run, &cap_arg)) {
		return 1;
	}

	struct snd_ctx snd_arg = {
		.ring = &ring
	};
	pthread_t snd_thread;
	if (pthread_create(&snd_thread, NULL, &snd_thread_run, &snd_arg)) {
		return 1;
	}

	pthread_join(cap_thread, NULL);
	pthread_join(snd_thread, NULL);

	return -1;
}
