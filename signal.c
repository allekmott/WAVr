/* signal.c
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

short *gen_sig(struct signal_spec *sigspec, void (*samplegen) (struct sample *)) {
	/* Check for valid frequency/sampleRate ratio */
	if (sigspec->sample_rate < sigspec->frequency * 2)
		printf("WARNING: With frequency of %f Hz, sample rate should be at least %i Hz\n",
			sigspec->frequency,
			(int) (sigspec->frequency * 2));

	short *buffer = alloc_buffer(sigspec);
	size_t bufferSize = bytesize_gen(sigspec);
	int sampleNo;

	/* time delta per sample */
	float tStep = 1.0f / (float) sigspec->sample_rate;

	/* create sample struct */
	struct sample sample;
	sample.sigspec = sigspec;

	/* gen dat sine */
	for (sampleNo = 0; sampleNo < bufferSize; sampleNo++) {
		/* time, relative to current sample */
		float t = tStep * sampleNo;
		
		sample.t = t;
		sample.data = &(buffer[sampleNo]);
		samplegen(&sample);
	}

	return buffer;
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