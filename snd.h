#ifndef SND_H
#define SND_H
#include "ring.h"

struct snd_ctx {
	struct event_ring * ring;
};

void * snd_thread_run(void *);
#endif
