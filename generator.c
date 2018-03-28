/**
 * @file	generator.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Core signal generation routines
 */

#include <errno.h>
#include <math.h>

#include "util.h"
#include "signal.h"
#include "generator.h"

static int
generate_sine(struct signal_desc *sig, double *samples, int count);

static int
generate_square(struct signal_desc *sig, double *samples, int count);

static int
generate_triangle(struct signal_desc *sig, double *samples, int count);

static int render_samples_8bit(double *raw, void *rendered, int count);
static int render_samples_16bit(double *raw, void *rendered, int count);
static int render_samples_24bit(double *raw, void *rendered, int count);
static int render_samples_32bit(double *raw, void *rendered, int count);

/* mapping from waveform -> generator function */
static const signal_generator_t generators[] = {
	generate_sine,		/* WAVR_WAVEFORM_SINE */
	generate_square,	/* WAVR_WAVEFORM_SQUARE */
	generate_triangle	/* WAVR_WAVEFORM_TRIANGLE */
};

#define BUFFER_SIZE 4096 << 4

/* Generate signal & write to specified file */
int generate_signal(enum waveform waveform, struct signal_desc *sig) {
	signal_generator_t generator;
	sample_renderer_t renderer;
	unsigned long n_total_samples;

	double raw_samples[BUFFER_SIZE];
	void *rendered_samples;

	unsigned long i;

	generator = generators[waveform];

	switch (sig->format.bit_depth) {
		case SAMPLE_BIT_DEPTH_8:	renderer = render_samples_8bit; break;
		case SAMPLE_BIT_DEPTH_16:	renderer = render_samples_16bit; break;
		case SAMPLE_BIT_DEPTH_24:	renderer = render_samples_24bit; break;
		case SAMPLE_BIT_DEPTH_32:	renderer = render_samples_32bit; break;
		default: return -EINVAL;
	}

	rendered_samples = calloc(BUFFER_SIZE, (sig->format.bit_depth / 8));
	n_total_samples = (sig->format.sample_rate) * (sig->duration) * 1e-6;

	for (i = 0; i < n_total_samples; i += BUFFER_SIZE) {
		generator(sig, raw_samples, BUFFER_SIZE);
		renderer(raw_samples, rendered_samples, BUFFER_SIZE);
	}

	return 0;
};

/* Generate trigonometric sine wave */
static int
generate_sine(struct signal_desc *sig, double *samples, int count) {
	double multiplier;
	int i;

	multiplier = freq_mhz_to_hz(sig->frequency) / sig->format.sample_rate;

	for (i = 0; i < count; i++)
		*(samples + i) = sin((multiplier * i) + M_PI);

	return 0;
}

/* Generate square wave */
static int
generate_square(struct signal_desc *sig, double *samples, int count) {
	return 0;
}

/* Generate trianglw wave */
static int
generate_triangle(struct signal_desc *sig, double *samples, int count) {
	return 0;
}

static int render_samples_8bit(double *raw, void *rendered, int count) {
	unsigned char *out_samples;
	int i;

	out_samples = (unsigned char *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (unsigned char) ((*raw + i) * (double) CHAR_MAX);

	return 0;
}

static int render_samples_16bit(double *raw, void *rendered, int count) {
	short *out_samples;
	int i;

	/* 16bit -> short */
	out_samples = (short *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (short) (*(raw + i) * (double) SHRT_MAX);

	return 0;
}

static int render_samples_24bit(double *raw, void *rendered, int count) {
	char *out_samples;
	int i, i_val;

	out_samples = (char *) rendered;
	for (i = 0; i < count; i++) {
		/* since 24-bit isn't symmetric, this one's a bit weird */
		i_val = (int)(*(raw + i) * (double) (INT_MAX >> 8));

		/* copy lower three bytes of in into buffer */
		memcpy((rendered + (i * 3)), ((char *) &i_val) + 1, sizeof(char) * 3);
	}

	return 0;
}

static int render_samples_32bit(double *raw, void *rendered, int count) {
	float *out_samples;
	int i;

	out_samples = (float *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (float) *(raw + i);

	return 0;
}
