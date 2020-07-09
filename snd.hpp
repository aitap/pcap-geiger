#ifndef SND_H
#define SND_H

#include <boost/lockfree/spsc_queue.hpp>
#include <portaudio.h>

class sound_thread {
	using queue = boost::lockfree::spsc_queue<unsigned int>;
	PaStream *stream;
	queue & q;
	static int cb(
		const void *input, void *output, unsigned long count,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData
	);
public:
	sound_thread(queue &);
	void run();
	~sound_thread();
};

void * snd_thread_run(void *);
#endif
