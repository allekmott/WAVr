/**
 * @file	generator.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Core signal generation routines
 */

#include <errno.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "util.h"
#include "signal.h"
#include "generator.h"

/* generators */
static int generate_sine(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset);

static int generate_square(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset);

static int generate_triangle(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset);

static int generate_sawtooth(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset);

/* renderers */
static int render_samples_8bit(double *raw, void *rendered,
		unsigned int count);

static int render_samples_16bit(double *raw, void *rendered,
		unsigned int count);

static int render_samples_24bit(double *raw, void *rendered,
		unsigned int count);

static int render_samples_32bit(double *raw, void *rendered,
		unsigned int count);

/* mapping from waveform -> generator function */
static const signal_generator_t generators[] = {
	generate_sine,		/* WAVEFORM_SINE */
	generate_square,	/* WAVEFORM_SQUARE */
	generate_triangle,	/* WAVEFORM_TRIANGLE */
	generate_sawtooth	/* WAVEFORM_SAWTOOTH */
};
#define waveform_generator(waveform) generators[(waveform) - 1]

enum waveform str_to_waveform(const char *s_waveform) {
	int n_waveform;

	if (sscanf(s_waveform, "%i", &n_waveform)) {
		/* already provided numeric value */
		if (n_waveform >= WAVEFORM_START_VAL && n_waveform <= WAVEFORM_END_VAL)
			return n_waveform;
		else
			return WAVEFORM_INVAL;
	} else {
		/* not provided numeric value, compare strings */
		for (n_waveform = WAVEFORM_START_VAL;
				n_waveform <= WAVEFORM_END_VAL;
				n_waveform++) {
			if (!strcmp(waveform_name(n_waveform), s_waveform))
				return n_waveform;
		}

		/* didn't match anything */
		return WAVEFORM_INVAL;
	}
}

#define BUFFER_SIZE 4096

/* Generate signal & write to specified file */
int generate_signal(enum waveform waveform, struct signal_desc *sig,
			void *output_dest, generator_cb_t writeout) {
	signal_generator_t generator;
	sample_renderer_t renderer;

	unsigned int n_total_samples;

	double raw_samples[BUFFER_SIZE];
	void *rendered_samples;

	size_t sample_size, rendered_buffer_size;
	unsigned int i, remaining;

	generator = waveform_generator(waveform);

	/* pick renderer for signal's bit depth */
	switch (sig->format.bit_depth) {
		case SAMPLE_BIT_DEPTH_8:	renderer = render_samples_8bit; break;
		case SAMPLE_BIT_DEPTH_16:	renderer = render_samples_16bit; break;
		case SAMPLE_BIT_DEPTH_24:	renderer = render_samples_24bit; break;
		case SAMPLE_BIT_DEPTH_32:	renderer = render_samples_32bit; break;
		default: return -EINVAL;
	}

	sample_size = sig->format.bit_depth / 8;
	rendered_buffer_size = sample_size * BUFFER_SIZE;

	rendered_samples = calloc(BUFFER_SIZE, sample_size);
	n_total_samples = (sig->format.sample_rate) * (sig->duration) * 1e-6;

	for (i = 0; (i + BUFFER_SIZE) < n_total_samples; i += BUFFER_SIZE) {
		/* printf("%i\n", i); */

		/* generate samples */
		generator(sig, raw_samples, BUFFER_SIZE, i);

		/* convert samples to specified format */
		renderer(raw_samples, rendered_samples, BUFFER_SIZE);

		if (writeout(output_dest, rendered_samples, BUFFER_SIZE) < 0) {
			return -1;
		}

		/* flush buffers */
		coolzero(raw_samples, sizeof(double) * BUFFER_SIZE);
		coolzero(rendered_samples, rendered_buffer_size);
	}

	/* pick up any stragglers */
	if (i < n_total_samples) {
		remaining = n_total_samples - i;
		/* printf("+%i\n", remaining); */

		generator(sig, raw_samples, remaining, i);
		renderer(raw_samples, rendered_samples, remaining);

		if (writeout(output_dest, rendered_samples, remaining) < 0) {
			return -1;
		}
	}

	free(rendered_samples);
	return 0;
};

#define signal_t_multiplier(t)	\
		(freq_mhz_to_hz((sig)->frequency) / (sig)->format.sample_rate)

/* Generate trigonometric sine wave */
static int generate_sine(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset) {
	double t_multiplier;
	unsigned int i;

	t_multiplier = signal_t_multiplier(sig);

	for (i = 0; i < count; i++)
		*(samples + i) = wave_sine(t_multiplier * (i + offset));

	return 0;
}

/* Generate square wave */
static int generate_square(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset) {
	double t_multiplier;
	unsigned int i;

	t_multiplier = signal_t_multiplier(sig);

	for (i = 0; i < count; i++)
		*(samples + i) = wave_square(t_multiplier * (i + offset));

	return 0;
}

/* Generate trianglw wave */
static int generate_triangle(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset) {
	double t_multiplier;
	unsigned int i;

	t_multiplier = signal_t_multiplier(sig);

	for (i = 0; i < count; i++)
		*(samples + i) = wave_triangle(t_multiplier * (i + offset));

	return 0;
}

/* generate sawtooth wave */
static int generate_sawtooth(struct signal_desc *sig,
		double *samples, unsigned int count, unsigned int offset) {
	double t_multiplier;
	unsigned int i;

	t_multiplier = signal_t_multiplier(sig);

	for (i = 0; i < count; i++)
		*(samples + i) = wave_sawtooth(t_multiplier * (i + offset));

	return 0;
}

static int render_samples_8bit(double *raw, void *rendered,
		unsigned int count) {
	char *out_samples;
	unsigned int i;

	out_samples = (char *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (char) ((*(raw + i) + 1.0) * (double) SCHAR_MAX);

	return 0;
}

static int render_samples_16bit(double *raw, void *rendered,
		unsigned int count) {
	short *out_samples;
	unsigned int i;

	/* 16bit -> short */
	out_samples = (short *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (short) ((*(raw + i)) * (double) SHRT_MAX);

	return 0;
}

static int render_samples_24bit(double *raw, void *rendered,
		unsigned int count) {
	char *out_samples;
	unsigned int i;
	int i_val;

	out_samples = (char *) rendered;
	for (i = 0; i < count; i++) {
		/* since 24-bit isn't symmetric, this one's a bit weird */
		i_val = (int) ((*(raw + i)) * (double) WAVR_INT24_MAX);

		/* copy lower three bytes of in into buffer */
		memcpy((rendered + (i * 3)), &i_val, sizeof(char) * 3);
	}

	return 0;
}

static int render_samples_32bit(double *raw, void *rendered,
		unsigned int count) {
	int *out_samples;
	unsigned int i;

	out_samples = (int *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (int) ((*(raw + i)) * (double) INT_MAX);

	return 0;
}
