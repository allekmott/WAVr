/**
 * @file	signal.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	28 March 2018
 * @brief	Routines for managing signals & signal properties
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <math.h>
#ifdef __linux__
#include <tgmath.h>
#endif /* __linux__ */

#include "signal.h"

enum sample_rate str_to_sample_rate(const char *s_sample_rate) {
	float n_sample_rate_f;	/* sample rate in kHz */
	int n_sample_rate_i;	/* sample rate in Hz */

	if (sscanf(s_sample_rate, "%f", &n_sample_rate_f)) {
		/* float (kHz), convert to int (Hz) */
		if (n_sample_rate_f < 1000.0f)
			n_sample_rate_i = (int) (n_sample_rate_f * 1.0e3f);
		else
			n_sample_rate_i = (int) (n_sample_rate_f);
	} else {
		/* didn't parse into number, invalid */
		return SAMPLE_RATE_INVAL;
	}

	switch (n_sample_rate_i) {
		case SAMPLE_RATE_32K:	return SAMPLE_RATE_32K;
		case SAMPLE_RATE_44_1K:	return SAMPLE_RATE_44_1K;
		case SAMPLE_RATE_48K:	return SAMPLE_RATE_48K;
		case SAMPLE_RATE_88_2K:	return SAMPLE_RATE_88_2K;
		case SAMPLE_RATE_96K:	return SAMPLE_RATE_96K;
		case SAMPLE_RATE_192K:	return SAMPLE_RATE_192K;
		default:				return SAMPLE_RATE_INVAL; /* not in enum */
	}
}

enum sample_bit_depth str_to_bit_depth(const char *s_sample_bit_depth) {
	int n_sample_bit_depth;

	if (!sscanf(s_sample_bit_depth, "%i", &n_sample_bit_depth))
		return SAMPLE_BIT_DEPTH_INVAL;

	switch (n_sample_bit_depth) {
		case SAMPLE_BIT_DEPTH_8:	return SAMPLE_BIT_DEPTH_8;
		case SAMPLE_BIT_DEPTH_16:	return SAMPLE_BIT_DEPTH_16;
		case SAMPLE_BIT_DEPTH_24:	return SAMPLE_BIT_DEPTH_24;
		case SAMPLE_BIT_DEPTH_32:	return SAMPLE_BIT_DEPTH_32;
		default:					return SAMPLE_BIT_DEPTH_INVAL;
	}
}

void dump_samples(void *samples, unsigned int count,
		enum sample_bit_depth bit_depth) {
	unsigned int i, b, per_line;
	int sample_size, n_chars;

	if (samples == NULL)
		return;

	sample_size = bit_depth / 8;
	n_chars = sample_size * 2;

	per_line = (80 / (n_chars + 1));

	for (i = 0; i < count; i++) {
		if (i % per_line == 0)
			printf("\n");
		else
			printf(" ");

		for (b = 0; b < sample_size; b++)
			printf("%02x",
					*((unsigned char *) (samples + (i * sample_size) + b)));
	}
}

int render_samples(struct signal_desc *sig, double *raw, unsigned int count) {
	size_t sample_size;
	sample_renderer_t renderer;

	/* should have anything with sub-byte samples (yet) */
	char *samples;

	unsigned int i;

	samples = (char *) raw;
	sample_size = signal_bytes_per_sample(sig);

	renderer = signal_renderer(sig);

	for (i = 0; i < count; i++) {
		renderer(*(raw + i), (void *) samples);
		samples += sample_size;
	}

	return 0;
}

void render_sample_8bit(double raw, void *rendered) {
	*((char *) rendered) = (char) ((raw + 1.0) * (double) SCHAR_MAX);
}

void render_sample_16bit(double raw, void *rendered) {
	*((short *) rendered) = (short) (raw * (double) SHRT_MAX);
}

void render_sample_24bit(double raw, void *rendered) {
	int val;

	val = (int) (raw * (double) WAVR_INT24_MAX);
	memcpy(rendered, &val, sizeof(char) * 3);
}

void render_sample_32bit(double raw, void *rendered) {
	*((int *) rendered) = (int) (raw * (double) INT_MAX);
}
