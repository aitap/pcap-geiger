#include <stdio.h>
#include <unistd.h>
#include <portaudio.h>

#include "snd.h"

#ifndef TICK_SAMPLES
	#define TICK_SAMPLES (44100 * 32 / 1000) // 32 ms
#endif

#ifndef MAX_EVENT
	#define MAX_EVENT 32000
#endif

static int cb(
	const void *input, void *output, unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData
) {
    (void)input, (void)timeInfo, (void)statusFlags;
	struct event_ring * ring = userData;
	float *out = output;
	unsigned int ev;

	for (unsigned long i = 0; i < frameCount; ++i) {
		if (event_read(ring, &ev)) {
			ev = ev > MAX_EVENT ? MAX_EVENT : ev;
			for (unsigned long j = 0; j < TICK_SAMPLES/4 && i < frameCount; ++j, ++i) {
				out[i] = 0;
			}
			for (unsigned long j = 0; j < TICK_SAMPLES/2 && i < frameCount; ++j, ++i) {
				out[i] = (float)(ev) / MAX_EVENT;
			}
			#if 0
			for (unsigned long j = 0; j < TICK_SAMPLES/2 && i < frameCount; ++j, ++i) {
				out[i] = (float)j * 2. / TICK_SAMPLES * (float)(ev) / MAX_EVENT;
			}
			for (unsigned long j = 0; j < TICK_SAMPLES/2 && i < frameCount; ++j, ++i) {
				out[i] = 1 - (float)j * 2. / TICK_SAMPLES * (float)(ev) / MAX_EVENT;
			}
			#endif
			for (unsigned long j = 0; j < TICK_SAMPLES/4 && i < frameCount; ++j, ++i) {
				out[i] = 0;
			}
		} else {
			for (unsigned long j = 0; j < TICK_SAMPLES && i < frameCount; ++j, ++i) {
				out[i] = 0;
			}
		}
	}
	return 0;
}

void * snd_thread_run(void * arg) {
	struct snd_ctx * ctx = arg;
	PaError err = Pa_Initialize();
	if (err != paNoError) return NULL;

	PaStream *stream;
	err = Pa_OpenDefaultStream( &stream,
			0,          /* no input channels */
			1,          /* stereo output */
			paFloat32,  /* 32 bit floating point output */
			44100,
			TICK_SAMPLES * 10,
			&cb, /* this is your callback function */
			ctx->ring );
	if (err != paNoError) return NULL;

	err = Pa_StartStream(stream);
	if (err != paNoError) return NULL;

	for (;;) Pa_Sleep(3600);
	/* Unreachable */
	return NULL;
}
