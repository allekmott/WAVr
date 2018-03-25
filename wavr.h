/* wavr.h
 * Declares argument struct and all argument
 * handling functions.
 * Created: 14 January 2016
 */

#ifndef __WAVR_H__
#define __WAVR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <math.h>

#include "wav.h"

#define WAVR_VERSION "0.6.0"

enum sample_rate {
	SAMPLE_RATE_32000,
	SAMPLE_RATE_44100,
	SAMPLE_RATE_48000,
	SAMPLE_RATE_96000,
	SAMPLE_RATE_192000
};

enum input_method {
	INPUT_NONE, /* => sample generation */
	INPUT_STDIN,
	INPUT_FILE
};

static const char *WAVR_HELP_MSG = "Misc Options:\n\
-h			Display help message (what you just did)\n\
\n\
Output Options:\n\
-o <out_file>		Output file (default = wavr.out.wav)\n\
-p			Dump stamples to console\n\
\n\
Generator Options:\n\
-d <seconds>		Duration of signal to be generated\n\
-f <frequency>		Frequency of signal to be generated\n\
-t <thread_count>	Number of worker threads to use\n\
-w <waveform>		Type of signal to generate (sine, square, triange)";

/* float seconds -> long nanoseconds */
#define time_stous(n_time_s) (round((n_time_s) * 1e6))
#define time_ustos(n_time_us) ((n_time_us) * 1e-6)

/* parse string into floating point frequency value
 * returns -1.0 on failure */
float str_to_freq(const char *s_freq);

/* parse string time (seconds) into numeric time (microseconds)
 * returns 0 on failure */
unsigned long str_to_us_time(const char *s_time);

/* convert numeric time (microseconds) into string time (seconds) */
char *us_time_to_str(unsigned long n_time, char *s_time);

/* error/warning logging */
#define lame(args...) fprintf(stderr, args)
#define super_lame(args...) { fprintf(stderr, args); exit(errno); }
#define lame_error(msg) fprintf(stderr, "%s: %s\n", (msg), strerror(errno));

#endif /* __WAVR_H__ */

