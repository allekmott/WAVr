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
enum waveform {
	WAVEFORM_SINE,		/* sine wave generator */
	WAVEFORM_SQUARE,	/* square wave generator */
	WAVEFORM_TRIANGLE	/* triangle wave generator */
};

/* Generates & renders samples using the provided generator function */
int generate_signal(enum waveform waveform, struct signal_desc *sig);

/* ------------------------------ GENERATORS ------------------------------- */

/* Generic signal generator; Takes in description of signal, pointer to sample
 * buffer, and # of samples to be generated */
typedef int (*signal_generator_t) (struct signal_desc *, double *, int);

/* Generic sample renderer; Takes in pointers to pre-rendered &
 * post-rendered sample buffers & number of samples to render */
typedef int (*sample_renderer_t) (double *, void *, int);

#endif /* __WAVR_GENERATOR_H__ */
