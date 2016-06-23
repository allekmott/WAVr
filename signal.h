/* signal.h
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

#ifndef SIGNAL_H
#define SIGNAL_H

/* Data structure to store sample information
 * Keeps track of sample's position in signal
 */
struct sample {
	float t;
	struct signal_spec *sigspec;
	short *data;
};

/* Data structure to house thread-relevant
 * information for signal workers.
 */
struct sampleworker_data {
	short *buffer; /* sample buffer */
	struct signal_spec *sigspec; /* struct containing signal information */
	int start_i; /* index to start at within buffer */
	int i_step; /* number of indicies to jump b/w samples */
	float t_step; /* time delta b/w samples */
	int total_samples; /* number of samples to generate */
	void (*generator) (struct sample *); /* generator function */
};

/* Generate signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *gen_sig(struct signal_spec *sigspec, void (*samplegen) (struct sample *), int thread_count);

/* Worker thread for sample generation
 * generates samples within thread bounds
 */
void *sample_worker(void *genspec);

/* Generate samples with sinusoidal function
 */
void samplegen_sine(struct sample *sample);

/* Parse signal from provided input file
 * (stdin by default)
 */
short *parse_sig(struct signal_spec *sigspec, FILE *in);

/* Calculate size of data segment (array of samples) to be generated.
 */
size_t bytesize_gen(struct signal_spec *sigspec);

/* Allocate buffer for signal with given signal_spec
 */
short *alloc_buffer(struct signal_spec *sigspec);

/* Dump raw samples to standard output */
void sample_dump(short *buffer, struct signal_spec *sigspec);

#endif