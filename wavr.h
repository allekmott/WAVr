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
-w <waveform>		Type of waveform to generate, name or value\n\
\n\
			sine (1) | square (2) | triangle (3)\n\
\n\
-f <frequency>		Frequency of signal to be generated (Hz)\n\
-d <duration>		Duration of signal to be generated (s)\n\
-s <sample_rate>	Sample rate in kHz or Hz\n\
\n\
			32000 (32.0) | 44100 (44.1) | 48000 (48.0) |\n\
			88200 (88.2) | 96000 (96.0) | 192000 (192.0) \n\
\n\
-b <bit_depth>		Sample bit depth\n\
\n\
			8 | 16 | 24 | 32\n\
\n\
-t <thread_count>	Number of worker threads to use";

#endif /* __WAVR_H__ */

