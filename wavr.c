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

#define WAVR_VERSION "0.1.4"

int main(int argc, char *argv[]) {
	printf("WAVr v%s\n", WAVR_VERSION);

	struct signal_spec sigspec = {DEFAULT_SAMPLE_RATE,
		DEFAULT_SIGNAL_FREQUENCY,
		DEFAULT_SIGNAL_DURATION};

	struct wavr_args args;
	args.out_filename = DEFAULT_OUTFILE;
	args.sample_dump = 0;
	args.sample_stdin = 0;
	args.sigspec = &sigspec;

	handle_args(&args, argc, argv);
	
	short *samples;

	if (args.sample_stdin) {
		/* Parse samples from stdin */
		printf("Reading samples in from stdin...\n");
		samples = parse_sig(&sigspec, stdin);
		exit(1);
	} else {
		/* Generate samples */
		printf("Generating samples...\n");
		samples = gen_sig(&sigspec, samplegen_sine);
		printf("Finished sample generation.\n");
	}

	printf("Writing to %s...\n", args.out_filename);
	FILE *out_file = init_wav_file(args.out_filename, &sigspec);

	//printf("File opened\n");
	size_t dataSize = bytesize_gen(&sigspec);
	fwrite(samples, dataSize, 1, out_file);

	if (!args.sample_dump)
		printf("Write complete, exiting.\n");
	else {
		int numSamples = sigspec.duration * sigspec.sample_rate;
		int curSample;
		for (curSample = 0; curSample < numSamples; curSample++) {
			printf("0x%x\t", samples[curSample]);
			if (curSample % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}

	free(samples);
	fclose(out_file);
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

	while ((c = getopt(argc, argv, "o:dt:f:ls:ih")) != -1) {
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
				if (args->sigspec->duration <= 0.0f)
					usage(argv[0]);
					exit(0);
				break;

			/* set frequency of waveform */	
			case 'f': 
				args->sigspec->frequency = atof(optarg);
				if (args->sigspec->frequency <= 0.0f)
					usage(argv[0]);
					exit(0);
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

			/* input sample values from stdin */
			case 'i':
				printf("Parsing samples from standard input\n");
				args->sample_stdin = 1;
				break;

			case 'h': help(argv[0]); break;

			/* wtf */
			case '?': usage(argv[0]); exit(0); break;
		}
	}
}