/* samplegen.h
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdlib.h>

#include "wav.h"

#ifndef SAMPLEGEN_H
#define SAMPLEGEN_H

/* Generate signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *gen_sig(struct signal_spec *sigspec);

/* Calculate size of data segment (array of samples) to be generated.
 */
size_t bytesize_gen(struct signal_spec *sigspec);

#endif