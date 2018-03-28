/* wavr.h
 * Declares argument struct and all argument
 * handling functions.
 * Created: 14 January 2016
 */

#ifndef __WAVR_H__
#define __WAVR_H__

#define WAVR_VERSION "0.6.0"

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
-p			Dump samples to console\n\
\n\
Generator Options:\n\
-w <waveform>		Type of signal to generate (sine, square, triange)\n\
-f <frequency>		Frequency of signal to be generated\n\
-d <duration>		Duration of signal to be generated in seconds\n\
-s <sample_rate>	Sample rate in kHz or Hz (44.1, 96000, ...)\n\
-b <bit_depth>		Sample bit depth (8, 16, 24, or 32)\n\
-t <thread_count>	Number of worker threads to use";

#endif /* __WAVR_H__ */

