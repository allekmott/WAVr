/**
 * @file	signal.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	28 March 2018
 * @brief	Routines for managing signals & signal properties
 */

#include <stdio.h>

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
