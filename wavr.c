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

static int wav_file_append_samples(void *file,
		void *sammples, unsigned int count);

int main(int argc, char *argv[]) {
	struct wav_file *file;
	struct signal_desc sig;
	enum waveform waveform;

	char flag;
	extern int optind, optopt;

	char dump_samples;

	char *output_path;

	coolzero(&sig, sizeof(struct signal_desc));

	/* defaults */
	output_path		= "wavr_out.wav";
	waveform		= WAVEFORM_SINE;	/* sine wave */

	sig.frequency 	= 1e6;		/* 1 kHz */
	sig.amplitude	= INT_MAX;	/* 100% volume */
	sig.duration	= 1e6;		/* 1 second */

	sig.format.sample_rate	= SAMPLE_RATE_44_1K;	/* 44.1 kHz */
	sig.format.bit_depth	= SAMPLE_BIT_DEPTH_16;	/* 16-bit signed short */

	/* parse args */
	while ((flag = getopt(argc, argv, "o:pd:f:ls:b:i:cj:w:h")) != -1) {
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
			case 's':
				/* sample rate */
				if (!(sig.format.sample_rate = str_to_sample_rate(optarg))) {
					lame("Invalid sample rate: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'b':
				/* bit depth */
				if (!(sig.format.bit_depth = str_to_bit_depth(optarg))) {
					lame("Invalid bit depth: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'w':
				/* waveform */
				if (!(waveform = str_to_waveform(optarg))) {
					lame("Invalid waveform: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'i': /* TODO: reimplement this */	break;
			case 'c': /* TODO: reimplement this */	break;
			case 'j': /* TODO: reimplement this */	break;
			case 'h':
				printf("WAVr v%s\n\n%s\n", WAVR_VERSION, WAVR_HELP_MSG);
				return 0;
		}
	}
	argc -= optind;
	argv += optind;

	printf("WAVr v%s\n\n", WAVR_VERSION);

	printf("Output file: %s\n\n"
			"Waveform:    %s\n"
			"Duration:    %.2f s\n"
			"Frequency:   %.2f Hz\n"
			"Sample rate: %.01f kHz\n"
			"Bit depth:   %i bit\n"
			"Approx size: %lu b\n",
			output_path,
			waveform_name(waveform),
			time_us_to_s(sig.duration),
			freq_mhz_to_hz(sig.frequency),
			sig.format.sample_rate * 1.0e-3,
			sig.format.bit_depth,
			signal_size_bytes(&sig));

	printf("\nGenerating samples...\n");

	if ((file = wav_file_open(output_path, 1)) <= 0) {
		lame_error("unable to open output file");
		return errno;
	}

	generate_signal(waveform, &sig, file, wav_file_append_samples);
	wav_file_close(file);

	printf("Finished sample generation.\n");

	return 0;
}

static int wav_file_append_samples(void *file,
		void *samples, unsigned int count) {
	struct wav_file *wav_file;

	if (file == NULL)
		return -1;

	wav_file = (struct wav_file *) file;

	return wav_file_write_samples(wav_file,
			samples, count, wav_file_n_samples(wav_file));
}
