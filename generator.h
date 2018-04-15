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

#include "waveform.h"
#include "signal.h"

/* Generic signal generator; Takes in description of signal, pointer to sample
 * buffer, and # of samples to be generated, and the offset in samples */
typedef int (*signal_generator_t)
		(struct signal_desc *, double *, unsigned int, unsigned int);

/* Callback function for use in generator. Generally used for writing buffer
 * to file.
 *
 * Arguments: pointer to arbitrary target, sample buffer, sample count
 * Should return 0 on success, -1 on failure.
 */
typedef int (*generator_cb_t) (void *, void *, unsigned int);

/* Generates & renders samples using the provided generator function */
int generate_signal(enum waveform waveform, struct signal_desc *sig,
		void *output_dest, generator_cb_t writeout);

#endif /* __WAVR_GENERATOR_H__ */
