/* samplegen.h
 * Mathematically generates signals
 * Created: 20 Dec 2015
 */

#include <stdlib.h>

#ifndef _SIGFAPPER_H
#define _SIGFAPPER_H

/* Generate signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *gen_sig(float frequency, float duration, int sampleRate);

/* Calculate size of data segment (array of samples) to be generated.
 */
size_t bytesize_gen(float duration, int sampleRate);

#endif