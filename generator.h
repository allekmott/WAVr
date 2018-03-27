/**
 * @file	generator.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	22 March 2018
 * @brief	Declarations of types & routines to be utilised in signal
 *			generation.
 */

#ifndef __WAVR_GENERATOR_H__
#define __WAVR_GENERATOR_H__

#include <limits.h>

#include "signal.h"

/* available waveform types */
enum wavr_waveform {
	WAVR_WAVEFORM_SINE,		/* sine wave generator */
	WAVR_WAVEFORM_SQUARE,	/* square wave generator */
	WAVR_WAVEFORM_TRIANGLE	/* triangle wave generator */
};

/* Generates & renders samples using the provided generator function */
int
wavr_generate_signal(enum wavr_waveform waveform, struct wavr_signal *sig);

/* ------------------------------ GENERATORS ------------------------------- */

/* signal generation function; utilises */
typedef int (*wavr_signal_generator_t) \
	(struct wavr_signal *, unsigned long, unsigned long);

#endif /* __WAVR_GENERATOR_H__ */

