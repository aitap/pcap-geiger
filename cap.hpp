#ifndef CAP_H
#define CAP_H

#include <boost/lockfree/spsc_queue.hpp>
#include <pcap/pcap.h>
#include <thread>

struct cap_ctx {
	const char *source, *filter;
	struct event_ring * ring;
};

void * cap_thread_run(void *);

class capture_thread {
	using queue = typename boost::lockfree::spsc_queue<unsigned int>;
	queue & q;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t * cap;
	std::thread thr;
	static void cb(u_char *ring, const struct pcap_pkthdr *h, const u_char *);
public:
	capture_thread(const char * source, const char * filter, queue & q);
	void run();
	~capture_thread();
};

#endif
