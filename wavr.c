/**
 * @file	wavr.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	20 December 2015
 * @brief	Entry point for WAVr. Handles command line args & WAVz
 */

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

#include "util.h"
#include "wav.h"
#include "signal.h"
#include "generator.h"

#include "wavr.h"

static int append_samples(void *file, void *sammples, unsigned int count);
static int dump_and_append_samples(
		void *file, void *samples, unsigned int count);

static void set_audio_format(struct wav_file *file, struct signal_desc *sig);

int main(int argc, char *argv[]) {
	struct wav_file *file;
	struct signal_desc sig;
	enum waveform waveform;

	char flag;
	extern int optind, optopt;

	char dump_samples;
	generator_cb_t sample_handler;

	char *output_path;

	int res;
	clock_t t_start, t_end;

	coolzero(&sig, sizeof(struct signal_desc));

	/* defaults */
	output_path		= "wavr_out.wav";
	dump_samples	= 0;

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
			case 'p': dump_samples = 1; break;
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
			"Signal size: %lu bytes\n",
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

	set_audio_format(file, &sig);

	sample_handler = (dump_samples) ? dump_and_append_samples : append_samples;

	/* FIXME: if signal size < existing data size, truncate file */
	t_start = clock();
	res = generate_signal(waveform, &sig, file, sample_handler);
	t_end = clock();

	if (res < 0) {
		lame_error("Signal generation failed");
		wav_file_close(file);

		return -1;
	}

	wav_file_close(file);

 	printf("\nfin. %.02lfs elapsed\n", time_clocks_to_s(t_end - t_start));

	return 0;
}

/* Append samples to a target wav_file. Used as signal generator callback */
static int append_samples(void *file, void *samples, unsigned int count) {
	struct wav_file *wav_file;

	if (file == NULL)
		return -1;

	wav_file = (struct wav_file *) file;

	return wav_file_write_samples(wav_file,
			samples, count, wav_file_n_samples(wav_file));
}

/* dump samples to terminal in hex format and write to wav_file */
static int dump_and_append_samples(void *file,
		void *samples, unsigned int count) {
	struct wav_file *wav_file;

	wav_file = (struct wav_file *) file;
	dump_samples(samples, count, wav_file_bit_depth(wav_file));

	return append_samples(file, samples, count);
}

/* Merge signal_desc properties into corresponding wav_audio_format fields
 * in wav_file */
static void set_audio_format(struct wav_file *file, struct signal_desc *sig) {
	struct wav_audio_format *fmt;

	if (file == NULL)
		return;

	fmt = wav_file_format(file);
	fmt->sample_rate = sig->format.sample_rate;
	fmt->bit_depth = sig->format.bit_depth;

	/* derived from parameters above */
	fmt->byte_rate = fmt->n_channels * fmt->sample_rate * (fmt->bit_depth / 8);
	fmt->block_align = fmt->n_channels * (fmt->bit_depth / 8);
}
