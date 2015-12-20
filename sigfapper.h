/* sigfapper.c
 * Faps a sig
 * Created: 20 Dec 2015
 */

#include <stdlib.h>

#ifndef _SIGFAPPER_H
#define _SIGFAPPER_H

/* Fap (generate) a signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *fap_sig(int frequency, float duration, int sampleRate);
size_t bytesize_gen(float duration, int sampleRate);

#endif