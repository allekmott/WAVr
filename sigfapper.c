/* sigfapper.c
 * Faps a sig
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sigfapper.h"

short *fap_sig(int frequency, float duration, int sampleRate) {
	int bufferSize = duration * sampleRate;

	if (sampleRate < frequency * 2)
		printf("WARNING: With frequency of %i Hz, sample rate should be at least %i Hz\n",
			frequency,
			frequency * 2);

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

		buffer[sample] = (short) sin((float) frequency * (2.0f * M_PI) * t);
	}

	return buffer;
}