#ifndef CAP_H
#define CAP_H
#include "ring.h"

struct cap_ctx {
	const char *source, *filter;
	struct event_ring * ring;
};

void * cap_thread_run(void *);
#endif
