#ifndef CAP_H
#define CAP_H

#include <boost/lockfree/spsc_queue.hpp>
#include <future>
#include <pcap/pcap.h>

class capture_thread {
	using queue = typename boost::lockfree::spsc_queue<unsigned int>;
	queue & q;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t * cap;
	static void cb(u_char *ring, const struct pcap_pkthdr *h, const u_char *);
public:
	capture_thread(const char * source, const char * filter, queue & q);
	std::future<void> run();
	~capture_thread();
};

#endif
