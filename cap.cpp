#include <stdexcept>

#include "cap.hpp"

void capture_thread::cb(u_char *ring, const struct pcap_pkthdr *h, const u_char *bytes) {
	(void)bytes;
	((capture_thread::queue*)ring)->push(h->len);
}


capture_thread::capture_thread(
	const char * source, const char * filter_, queue & q_
) :q(q_) {
#ifdef _WIN32
	if (pcap_wsockinit()) {
		throw std::runtime_error("pcap_wsockinit failed");
	}
#endif

	cap = pcap_create(source, errbuf);
	if (!cap) {
		throw std::runtime_error(errbuf);
	}

	try {
		/* NOTE: pcap_compile before pcap_activate results in error or segfault */
		if (pcap_activate(cap)) throw std::runtime_error(pcap_geterr(cap));

		struct bpf_program filter;
		if (pcap_compile(cap, &filter, filter_, 1, PCAP_NETMASK_UNKNOWN))
			throw std::runtime_error(pcap_geterr(cap));

		int sffail = pcap_setfilter(cap, &filter);
		pcap_freecode(&filter);
		if (sffail) throw std::runtime_error(pcap_geterr(cap));
	} catch (...) {
		this->~capture_thread();
		throw;
	}
}

void capture_thread::run() {
	thr = std::move(std::thread(pcap_loop, cap, -1, cb, (u_char*)&q));
}

capture_thread::~capture_thread() {
	pcap_close(cap);
}
