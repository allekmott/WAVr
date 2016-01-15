/* samplegen.c
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wav.h"
#include "samplegen.h"

size_t bytesize_gen(struct signal_spec *sigspec) {
	int bufferLength = sigspec->duration * sigspec->sample_rate;
	int sampleSize = sizeof(short);

	return (size_t) (bufferLength * sampleSize);
}

short *gen_sig(struct signal_spec *sigspec) {
	int bufferSize = bytesize_gen(sigspec);

	if (sigspec->sample_rate < sigspec->frequency * 2)
		printf("WARNING: With frequency of %f Hz, sample rate should be at least %i Hz\n",
			sigspec->frequency,
			(int) (sigspec->frequency * 2));

	short *buffer = malloc(bufferSize);
	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate memory for sample buffer\n"
			"\tDesired buffer size: %ib\n", bufferSize);
		exit(1);
	}


	int sample;

	/* time delta per sample */
	float tStep = 1.0f / (float) sigspec->sample_rate;

	/* gen dat sine */
	for (sample = 0; sample < bufferSize; sample++) {
		/* time, relative to current sample */
		float t = tStep * sample;

		buffer[sample] = (short) (10000.0f * sin(sigspec->frequency * (2.0f * M_PI) * t));
	}

	return buffer;
}

