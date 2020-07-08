#include <pcap/pcap.h>
//#include <stdio.h>

#include "cap.h"

#ifndef MAX_PKT_LEN
	#define MAX_PKT_LEN 65536
#endif

static void cb(u_char *ring, const struct pcap_pkthdr *h, const u_char *bytes) {
	(void)bytes;
	//fprintf(stderr, ".");
	event_write((struct event_ring *)ring, h->len);
}

static char errbuf[PCAP_ERRBUF_SIZE];

// TODO: differentiate between exit reasons
// TODO: maybe separate the initialisation

void * cap_thread_run(void * arg) {
	struct cap_ctx * ctx = arg;

#ifdef _WIN32
	if (pcap_wsockinit()) {
		return NULL;
	}
#endif
	pcap_t * cap = pcap_create(ctx->source, errbuf);
	if (!cap) {
		return NULL;
	}

	/* NOTE: pcap_compile before pcap_activate results in error or segfault */
	if (pcap_activate(cap)) goto cleanup;

	struct bpf_program filter;
	if (pcap_compile(cap, &filter, ctx->filter, 1, PCAP_NETMASK_UNKNOWN))
		goto cleanup;

	{
		int sffail = pcap_setfilter(cap, &filter);
		pcap_freecode(&filter);
		if (sffail) goto cleanup;
	}

	pcap_loop(cap, -1, cb, (u_char*)ctx->ring);

	/* Supposed to be normally unreachable */
cleanup:
	pcap_close(cap);
	return NULL;
}
