/**
 * @file	generator.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Core signal generation routines
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "util.h"
#include "waveform.h"
#include "signal.h"

#include "generator.h"

static int generate_samples(struct signal_desc *sig, wave_func_t wave_func,
		double *samples, unsigned int count, unsigned int offset);

#define BUFFER_SIZE (4096 << 4)

/* Generate signal & write to specified file */
int generate_signal(enum waveform waveform, struct signal_desc *sig,
			void *output_dest, generator_cb_t writeout) {
	wave_func_t wave_func;

	unsigned int n_total_samples;
	double samples[BUFFER_SIZE];

	unsigned int i, remaining;

	wave_func = waveform_func(waveform);
	n_total_samples = signal_n_samples(sig);

	for (i = 0; (i + BUFFER_SIZE) < n_total_samples; i += BUFFER_SIZE) {
		/* generate samples */
		generate_samples(sig, wave_func, samples, BUFFER_SIZE, i);

		/* convert samples to specified format */
		render_samples(sig, samples, BUFFER_SIZE);

		if (writeout(output_dest, samples, BUFFER_SIZE) < 0)
			return -1;

		/* flush buffers */
		coolzero(samples, sizeof(double) * BUFFER_SIZE);
	}

	/* pick up any stragglers */
	if (i < n_total_samples) {
		remaining = n_total_samples - i;

		generate_samples(sig, wave_func, samples, remaining, i);
		render_samples(sig, samples, remaining);

		if (writeout(output_dest, samples, remaining) < 0) {
			return -1;
		}
	}

	return 0;
};

static int generate_samples(struct signal_desc *sig, wave_func_t wave_func,
		double *samples, unsigned int count, unsigned int offset) {
	double t_multiplier;
	unsigned int i;

	t_multiplier = freq_mhz_to_hz(sig->frequency) / (sig->format.sample_rate);

	for (i = 0; i < count; i++)
		*(samples + i) = wave_func(t_multiplier * (i + offset));

	return 0;
}
