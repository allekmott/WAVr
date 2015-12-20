/* sigfapper.c
 * Faps a sig
 * Created: 20 Dec 2015
 */

#ifndef _SIGFAPPER_H
#define _SIGFAPPER_H

/* Fap (generate) a signal with
 * frequency (Hz)
 * duration (s)
 * sampleRate (Hz)
 */
short *fap_sig(int frequency, float duration, int sampleRate);

#endif