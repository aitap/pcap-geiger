#include "snd.hpp"

#include <stdexcept>

#ifndef TICK_SAMPLES
	#define TICK_SAMPLES (44100 * 4 / 1000) // 4 ms
#endif

#ifndef MAX_EVENT
	#define MAX_EVENT 2048
#endif

int sound_thread::cb(
	const void *input, void *output, unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *q
) {
    (void)input, (void)timeInfo, (void)statusFlags;
	float *out = (float*)output;
	unsigned int ev;

	for (unsigned long i = 0; i < frameCount; ++i) {
		if (((queue*)q)->pop(ev)) {
			ev = ev > MAX_EVENT ? MAX_EVENT : ev;
			for (unsigned long j = 0; j < TICK_SAMPLES && i < frameCount; ++j, ++i) {
				out[i] = (j == 0 || j == TICK_SAMPLES) ? 0 : (float)(ev) / MAX_EVENT;
			}
		} else {
			for (unsigned long j = 0; j < TICK_SAMPLES && i < frameCount; ++j, ++i) {
				out[i] = 0;
			}
		}
	}
	return 0;
}

sound_thread::sound_thread(queue & q_) : q(q_) {
	PaError err = Pa_Initialize();
	if (err != paNoError) throw std::runtime_error("Pa_Initialize failed");

	err = Pa_OpenDefaultStream(
		&stream,
		0, // no input
		1, // mono output
		paFloat32,
		44100,
		TICK_SAMPLES * 1024,
		&cb,
		(void*)&q
	);
	if (err != paNoError) throw std::runtime_error("Pa_OpenDefaultStream failed");
}

void sound_thread::run() {
	PaError err = Pa_StartStream(stream);
	if (err != paNoError) throw std::runtime_error("Pa_StartStream failed");
}

sound_thread::~sound_thread() {
	Pa_AbortStream(stream);
}
