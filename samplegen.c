/* samplegen.c
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "samplegen.h"

size_t bytesize_gen(float duration, int sampleRate) {
	int bufferLength = duration * sampleRate;
	int sampleSize = sizeof(short);

	return (size_t) (bufferLength * sampleSize);
}

short *gen_sig(float frequency, float duration, int sampleRate) {
	int bufferSize = bytesize_gen(duration, sampleRate);

	if (sampleRate < frequency * 2)
		printf("WARNING: With frequency of %f Hz, sample rate should be at least %i Hz\n",
			frequency,
			(int) (frequency * 2));

	short *buffer = malloc(sizeof(short) * bufferSize);
	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate memory for sample buffer\n"
			"\tDesired buffer size: %ib\n", bufferSize);
		exit(1);
	}


	int sample;

	/* time delta per sample */
	float tStep = 1.0f / (float) sampleRate;

	/* gen dat sine */
	for (sample = 0; sample < bufferSize; sample++) {
		/* time, relative to current sample */
		float t = tStep * sample;

		buffer[sample] = (short) (10000.0f * sin(frequency * (2.0f * M_PI) * t));
	}

	return buffer;
}

