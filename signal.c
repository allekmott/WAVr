/* signal.c
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "wav.h"
#include "signal.h"

size_t bytesize_gen(struct signal_spec *sigspec) {
	/* buffer length (samples) = duration (seconds) * sample rate (samples/sec) */
	size_t bufferLength = sigspec->duration * sigspec->sample_rate;

	/* sample size set at short (for now) */
	size_t sampleSize = sizeof(short);

	/* size of buffer (bytes) = buffer length (samples) * sample size (bytes/sample) */
	return (bufferLength * sampleSize);
}

short *alloc_buffer(struct signal_spec *sigspec) {
	short *buffer;

	/* (see bytesize_gen for calculation of buffer size) */
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
	/* Helpful printout */
	printf("\nSignal info:\n"
		"------------\n"
		"Frequency:\t%.2f Hz\n"
		"Duration:\t%.2fs\n"
		"Sample rate:\t%i Hz\n\n", sigspec->frequency, sigspec->duration, sigspec->sample_rate);

	/* Check for valid frequency/sampleRate ratio */
	if (sigspec->sample_rate < sigspec->frequency * 2)
		printf("WARNING: With frequency of %f Hz, sample rate should be at least %i Hz\n",
			sigspec->frequency,
			(int) (sigspec->frequency * 2));

	short *buffer = alloc_buffer(sigspec);
	size_t bufferSize = bytesize_gen(sigspec);

	int numberOfSamples = bufferSize / sizeof(short);
	int sampleNo;

	/* time delta per sample (in seconds) */
	float tStep = 1.0f / (float) sigspec->sample_rate;

	/* arrays of clock_t's corresponding to each thread */
	clock_t start_times[thread_count];

	/* aaaannnnndd heres where we start threading it up... */
	pthread_t workers[thread_count];

	int threadnum;
	struct sampleworker_data data[thread_count];

	/* create data structures, launch threads */
	for (threadnum = 0; threadnum < thread_count; threadnum++) {
		struct sampleworker_data *datum = &data[threadnum];

		/* index offset for thread */
		int ioffset = threadnum;

		datum->buffer = buffer;
		datum->sigspec = sigspec;
		datum->start_i = threadnum;
		datum->i_step = thread_count;
		datum->t_step = tStep;
		datum->total_samples = numberOfSamples;

		/* pass generator function to data struct */
		datum->generator = samplegen;

		printf("Start worker (%i)\n", threadnum + 1);

		/* register start time of thread */
		start_times[threadnum] = clock();
		pthread_create(&workers[threadnum], NULL, sample_worker, (void *) datum);
	}
	printf("\n");
	
	/* rejoin upon termination */
	for (threadnum = 0; threadnum < thread_count; threadnum++) {
		void *status;
		pthread_join(workers[threadnum], &status);
		
		/* calculate time difference */
		clock_t diff = clock() - start_times[threadnum];
		float elapsed = (float) diff / (float) CLOCKS_PER_SEC;

		printf("Worker exit (%i), %.2fs elapsed\n", threadnum + 1, elapsed);
	}
	printf("\n");

	return buffer;
}

void *sample_worker(void *sampleworker_data) {
	struct sampleworker_data *data = (struct sampleworker_data *) sampleworker_data;

	/* create sample struct */
	struct sample sample;
	sample.sigspec = data->sigspec;

	/* gen dat sine */
	int sampleNo;
	for (sampleNo = data->start_i; sampleNo < data->total_samples; sampleNo += data->i_step) {
		/* time, relative to current sample */
		float t = data->t_step * (data->start_i + sampleNo);
		
		sample.t = t;

		/* sample data points to location in buffer */
		sample.data = &(data->buffer[data->start_i + sampleNo]);
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