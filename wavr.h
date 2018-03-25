/* wavr.h
 * Declares argument struct and all argument
 * handling functions.
 * Created: 14 January 2016
 */

#ifndef __WAVR_H__
#define __WAVR_H__

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

#endif /* __WAVR_H__ */

