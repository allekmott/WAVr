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

	if (!(n_sample_bit_depth % 8 == 0)) {
		return SAMPLE_BIT_DEPTH_INVAL;
	} else if (n_sample_bit_depth < SAMPLE_BIT_DEPTH_START_VAL ||
			n_sample_bit_depth > SAMPLE_BIT_DEPTH_END_VAL) {
		return SAMPLE_BIT_DEPTH_INVAL;
	} else {
		return n_sample_bit_depth;
	}
}

float str_to_amplitude(const char *s_amplitude) {
	float n_amplitude;

	if (sscanf(s_amplitude, "%f", &n_amplitude)) {
		if (n_amplitude < 0.0f) {
			return -1.0;
		} else if (n_amplitude > 1.0f) {
			if (n_amplitude > 100.0f)
				return -1.0;

			n_amplitude /= 100.0f;
		}

		return n_amplitude;
	} else {
		return -1.0;
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

int render_samples(struct signal_desc *sig, double *buf, unsigned int count) {
	char *samples;
	float amplitude;

	int val;
	unsigned int i;

	samples = (char *) buf;
	amplitude = sig->amplitude;

	/* TODO: figure out a cleaner way of doing this */

	switch (sig->format.bit_depth) {
		case SAMPLE_BIT_DEPTH_8:
			/* actually unsigned char, so range is 0-255 vs -126-125 */
			for (i = 0; i < count; ++i, ++buf, ++samples)
				*samples = (char) (amplitude *
						(((*buf) + 1.0) * (double) SCHAR_MAX));
			break;

		case SAMPLE_BIT_DEPTH_16:
			/* signed 16-bit short */
			for (i = 0; i < count; ++i, ++buf, samples += 2)
				*((short *) samples) = (short) (amplitude *
						((*buf) * (double) SHRT_MAX));
			break;

		case SAMPLE_BIT_DEPTH_24:
			/* signed 24-bit int
			 * use a 32-bit int for calculation, then copy only 24-bits */
			for (i = 0; i < count; ++i, ++buf, samples += 3) {
				val = (int) (amplitude * ((*buf) * (double) WAVR_INT24_MAX));

				/* FIXME: this will probably break under big-endian */
				memcpy(samples, &val, sizeof(char) * 3);
			}
			break;

		case SAMPLE_BIT_DEPTH_32:
			/* not actually 32-bit float (here), but rather signed int */
			for (i = 0; i < count; ++i, ++buf, samples += 4)
				*((int *) samples) = (int) (amplitude *
						((*buf) * (double) INT_MAX));
			break;

		default:
			break;
	}

	return 0;
}
