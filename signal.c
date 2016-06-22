/* signal.c
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "wav.h"
#include "signal.h"

size_t bytesize_gen(struct signal_spec *sigspec) {
	size_t bufferLength = sigspec->duration * sigspec->sample_rate;
	size_t sampleSize = sizeof(short);

	return (bufferLength * sampleSize);
}

short *alloc_buffer(struct signal_spec *sigspec) {
	short *buffer;
	size_t bufferSize = bytesize_gen(sigspec);

	buffer = malloc(bufferSize);

	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate memory for sample buffer\n"
			"\tDesired buffer size: %zub\n", bufferSize);
		exit(1);
	}


	return buffer;
}

short *gen_sig(struct signal_spec *sigspec, void (*samplegen) (struct sample *), int thread_count) {
	/* Check for valid frequency/sampleRate ratio */
	if (sigspec->sample_rate < sigspec->frequency * 2)
		printf("WARNING: With frequency of %f Hz, sample rate should be at least %i Hz\n",
			sigspec->frequency,
			(int) (sigspec->frequency * 2));

	short *buffer = alloc_buffer(sigspec);
	size_t bufferSize = bytesize_gen(sigspec);
	int sampleNo;

	/* time delta per sample (in seconds) */
	float tStep = 1.0f / (float) sigspec->sample_rate;

	/* aaaannnnndd heres where we start threading it up... */
	// TODO thread dat shiz
	pthread_t workers[thread_count];

	/* attributes (to make joinable) */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int threadnum;
	int samples_per_thread = bufferSize/thread_count;
	// TODO finishhh
	for (threadnum = 0; threadnum < thread_count; threadnum++) {
		struct sampleworker_data data;
		data.buffer = &buffer[samples_per_thread * threadnum];
		data.sigspec = sigspec;
		data.t_step = tStep;

		/* last dude gets remainder */
		data.number_to_generate = ((threadnum + 1) == thread_count) ?
			samples_per_thread + (bufferSize % thread_count) : samples_per_thread;

		/* pass generator function to data struct */
		data.generator = samplegen;

		pthread_create(&workers[threadnum], &attr, sample_worker, (void *) &data);
	}
	/* free attr */
	pthread_attr_destroy(&attr);

	/* rejoin upon termination */
	for (threadnum = 0; threadnum < thread_count; threadnum++) {
		void *status;
		pthread_join(workers[threadnum], &status);
	}

	return buffer;
}

void *sample_worker(void *sampleworker_data) {
	struct sampleworker_data *data = (struct sampleworker_data *) sampleworker_data;

	/* create sample struct */
	struct sample sample;
	sample.sigspec = data->sigspec;

	/* gen dat sine */
	int sampleNo;
	for (sampleNo = 0; sampleNo < data->number_to_generate; sampleNo++) {
		/* time, relative to current sample */
		float t = data->t_step * sampleNo;
		
		sample.t = t;

		/* sample data points to location in buffer */
		sample.data = &(data->buffer[sampleNo]);
		data->generator(&sample);
	}
	return NULL;
}

void samplegen_sine(struct sample *sample) {
	*(sample->data) = (short) (10000.0f * sin(sample->sigspec->frequency * (2.0f * M_PI) * sample->t));
}

short *parse_sig(struct signal_spec *sigspec, FILE *in) {
	size_t bufferSize = bytesize_gen(sigspec);
	short *buffer = alloc_buffer(sigspec);

	/* wonky pointer shite */
	char *charBuffer = (char *) buffer;

	int curByte;
	for (curByte = 0; curByte < bufferSize; curByte++)
		fscanf(in, "%c", &charBuffer[curByte]);

	return buffer;
}

void sample_dump(short *buffer, struct signal_spec *sigspec) {
	int curByte;

	size_t bufferSize = bytesize_gen(sigspec);

	/* wonky pointer shite */
	char *charBuffer = (char *) buffer;

	for (curByte = 0; curByte < bufferSize; curByte++)
		printf("%c", charBuffer[curByte]);

	printf("\n");
}