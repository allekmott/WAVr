/* signal.h
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

#ifndef SIGNAL_H
#define SIGNAL_H

/* Generate signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *gen_sig(struct signal_spec *sigspec);

/* Parse signal from provided input file
 * (stdin by default)
 */
short *parse_sig(struct signal_spec *sigspec, FILE *in);

/* Calculate size of data segment (array of samples) to be generated.
 */
size_t bytesize_gen(struct signal_spec *sigspec);

#endif