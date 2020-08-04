#include <boost/lockfree/spsc_queue.hpp>
#include <chrono>
#include <thread>

#include "cap.hpp"
#include "snd.hpp"

#ifndef RING_SIZE
	#define RING_SIZE 1024
#endif

int main(int argc, char ** argv) {
	if (argc != 3) {
		printf("Usage: %s <pcap source> <pcap filter>\n", argv[0]);
		return 1;
	}

	boost::lockfree::spsc_queue<unsigned int> queue(RING_SIZE);

	capture_thread cap(argv[1], argv[2], queue);
	sound_thread snd(queue);

	auto retc = cap.run();
	snd.run();

	retc.get();

	return -1;
}
