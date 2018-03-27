/**
 * @file	wavr.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	20 December 2015
 * @brief	Entry point for WAVr. Handles command line args & WAVz
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "util.h"
#include "wav.h"
#include "signal.h"
#include "generator.h"

#include "wavr.h"

int main(int argc, char *argv[]) {
	struct wav_file *file;
	struct wavr_signal sig;
	enum wavr_waveform waveform;

	char flag;
	extern int optind, optopt;

	char dump_samples;

	char *output_path;

	coolzero(&sig, sizeof(struct wavr_signal));

	/* defaults */
	output_path		= "wavr_out.wav";
	waveform		= WAVR_WAVEFORM_SINE;	/* sine wave */

	sig.frequency 	= 1e6;					/* 1 kHz */
	sig.amplitude	= INT_MAX;				/* 100% volume */
	sig.duration	= 1e6;					/* 1 second */
	sig.sample_rate	= 44100;				/* 44.1 kHz */

	/* parse args */
	while ((flag = getopt(argc, argv, "o:pd:f:ls:i:cj:w:h")) != -1) {
		switch (flag) {
			case 'o': output_path = optarg; break;
			case 'p': /* TODO: dump samples */ break;
			case 'd':
				if (!(sig.duration = str_to_time_us(optarg))) {
					lame("Invalid duration value: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'f':
				if  (!(sig.frequency = str_to_freq_mhz(optarg))) {
					lame("Invalid frequency value: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'l': /* TODO: reimplement this */	break;
			case 's': /* TODO: reimplement this */	break;
			case 'i': /* TODO: reimplement this */	break;
			case 'c': /* TODO: reimplement this */	break;
			case 'j': /* TODO: reimplement this */	break;
			case 'w': /* TODO: reimplement this */	break;
			case 'h':
				printf("WAVr v%s\n\n%s\n", WAVR_VERSION, WAVR_HELP_MSG);
				return 0;
		}
	}
	argc -= optind;
	argv += optind;

	printf("WAVr v%s\n\n", WAVR_VERSION);

	printf("Output file: %s\nDuration:    %.2f s\nFrequency:   %.2f Hz\n",
			output_path,
			time_us_to_s(sig.duration),
			freq_mhz_to_hz(sig.frequency));

	printf("\nGenerating samples...\n");

	wavr_generate_signal(waveform, &sig);

	printf("Finished sample generation.\n");

	return 0;
}
