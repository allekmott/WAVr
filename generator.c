/**
 * @file	generator.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Core signal generation routines
 */

#include <math.h>

#include "util.h"
#include "signal.h"
#include "generator.h"

static int sine_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset);
static int square_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset);
static int triangle_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset);

/* mapping from waveform -> generator function */
static wavr_signal_generator_t generators[] = {
	sine_generator,		/* WAVR_WAVEFORM_SINE */
	square_generator,	/* WAVR_WAVEFORM_SQUARE */
	triangle_generator	/* WAVR_WAVEFORM_TRIANGLE */
};

#define WAVR_GEN_BUFFER_SIZE (4096)

/* Generate signal & write to specified file */
int wavr_generate_signal(
		enum wavr_waveform waveform, struct wavr_signal *sig) {
	wavr_signal_generator_t generator;
	double buf[WAVR_GEN_BUFFER_SIZE];

	int i;

	generator = generators[waveform];
	sig->samples = buf;
	sig->n_samples = (sig->duration * sig->sample_rate) * 1e-3;

	for (i = 0; i < sig->n_samples / WAVR_GEN_BUFFER_SIZE; i++)
		generator(sig, WAVR_GEN_BUFFER_SIZE, i * WAVR_GEN_BUFFER_SIZE);

	return 0;
}

/* Generate trigonometric sine wave */
static int sine_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset) {
	unsigned long i;
	double val;
	double multiplier = freq_mhz_to_hz(sig->frequency) / sig->sample_rate;

	printf("Generating %lu/%lu samples...\n", n_samples, sig->n_samples);

	for (i = 0; i < n_samples; i++) {
		val = sin((multiplier * i) + M_PI);
		printf("%.05f\n", val);
	}


	return 0;
}

/* Generate square wave */
static int square_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset) {
	return 0;
}

/* Generate trianglw wave */
static int triangle_generator(struct wavr_signal *sig,
		unsigned long n_samples, unsigned long offset) {
	return 0;
}
