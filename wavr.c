/* wavr.c
 * Main thingy
 * 5p4wn wav file
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wav.h"
#include "wavr.h"
#include "signal.h"

#define WAVR_VERSION "0.2.2"

int main(int argc, char *argv[]) {
	struct signal_spec sigspec = {DEFAULT_SAMPLE_RATE,
		DEFAULT_SIGNAL_FREQUENCY,
		DEFAULT_SIGNAL_DURATION};

	struct wavr_args args;
	/* args.in_filename only initialized if necessary */
	args.out_filename = DEFAULT_OUTFILE;
	args.sample_dump = 0;
	args.input = INPUT_NONE; /* generate samples by default */
	args.sigspec = &sigspec;

	struct WavFile *wav;

	handle_args(&args, argc, argv);

	if (!args.sample_dump)
		printf("WAVr v%s\n", WAVR_VERSION);

	switch (args.input) {
		case INPUT_NONE:
			/* Generate samples */
			wav = init_wav_file(&sigspec);

			if (!args.sample_dump)
				printf("\nGenerating samples...\n");
			wav->data = gen_sig(&sigspec, samplegen_sine);
			
			if (!args.sample_dump)
				printf("Finished sample generation.\n");
			break;
		case INPUT_STDIN:
			/* Parse samples from stdin */
			if (!args.sample_dump)
				printf("Reading samples in from stdin...\n");

			wav = init_wav_file(&sigspec);
			wav->data = parse_sig(&sigspec, stdin);
			break;
		case INPUT_FILE:
			/* Pull samples from input file */
			wav = read_wav_file(args.in_filename);
			sigspec_from_wav(wav, &sigspec);
			break;
		default: exit(1); /* lol? */
	}

	if (!args.sample_dump) {
		/* no dump, so writeout */
		write_wav_file(args.out_filename, wav);
	} else {
		sample_dump(wav->data, &sigspec);	
	}

	/* cleanup */
	free_wavfile(wav);
}

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
}

void help(const char *cmd) {
	usage(cmd);

	printf("Flags: \n"
		"-o <out_file>\t\tOutput wav to <out_file>\n"
		"-d\t\t\tDump samples to standard out (skip wav formatting)\n"
		"-t <duration>\t\tSet signal length to <duration> (s)\n"
		"-f <frequency>\t\tSet signal frequency to <frequency> Hz\n"
		"-s <sampleRate>\t\tSet signal sample rate to <sampleRate> Hz\n"
		"-l\t\t\tList available sample rates\n"
		"-i\t\t\tInput samples from standard input\n"
		"-h\t\t\tWhat you just did\n");

	exit(0);
}

void handle_args(struct wavr_args *args, int argc, char *argv[]) {
	int c;
	extern int optind, optopt;

	while ((c = getopt(argc, argv, "o:dt:f:ls:i:ch")) != -1) {
		switch (c) {
			/* set output filename */
			case 'o': args->out_filename = optarg; break;
			
			/* dump samples directly to command line
			 * completely bypasses wav formatting
			 */
			case 'd': args->sample_dump = 1; break;

			/* set duration of waveform */
			case 't':
				args->sigspec->duration = atof(optarg);
				if (args->sigspec->duration <= 0.0f) {
					usage(argv[0]);
					exit(0);
				}
				break;

			/* set frequency of waveform */	
			case 'f': 
				args->sigspec->frequency = atof(optarg);
				if (args->sigspec->frequency <= 0.0f) {
					usage(argv[0]);
					exit(0);
				}
				break;

			/* set waveform sample rate */
			case 's':
				args->sigspec->sample_rate = atoi(optarg);
				if (args->sigspec->sample_rate == 44100 ||
					args->sigspec->sample_rate == 48000 ||
					args->sigspec->sample_rate == 88200 ||
					args->sigspec->sample_rate == 96000)
						break;
				else
					printf("Invalid sample rate: %i Hz\n", args->sigspec->sample_rate);
					exit(1);
					break;

			/* list available sample rates */
			case 'l':
				printf("Available sample rates:\n\n"
					"44,100 Hz (44.1 kHz)\n"
					"48,000 Hz (48.0 kHz)\n"
					"88,200 Hz (88.2 kHz)\n"
					"96,000 Hz (96.0 kHz)\n");
				exit(0);
				break;

			case 'i':
				args->input = INPUT_FILE;
				args->in_filename = optarg;
				break;

			/* input sample values from stdin */
			case 'c':
				/*printf("Parsing samples from standard input\n");*/
				args->input = INPUT_STDIN;
				break;

			case 'h': help(argv[0]); break;

			/* wtf */
			case '?': usage(argv[0]); exit(0); break;
		}
	}
}