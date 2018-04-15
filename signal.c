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

int render_samples_8bit(double *raw, void *rendered,
		unsigned int count) {
	char *out_samples;
	unsigned int i;

	out_samples = (char *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (char) ((*(raw + i) + 1.0) * (double) SCHAR_MAX);

	return 0;
}

int render_samples_16bit(double *raw, void *rendered,
		unsigned int count) {
	short *out_samples;
	unsigned int i;

	/* 16bit -> short */
	out_samples = (short *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (short) ((*(raw + i)) * (double) SHRT_MAX);

	return 0;
}

int render_samples_24bit(double *raw, void *rendered,
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

int render_samples_32bit(double *raw, void *rendered,
		unsigned int count) {
	int *out_samples;
	unsigned int i;

	out_samples = (int *) rendered;
	for (i = 0; i < count; i++)
		*(out_samples + i) = (int) ((*(raw + i)) * (double) INT_MAX);

	return 0;
}
