/* wavr.c
 * Main thingy
 * 5p4wn wav file
 * Created: 20 Dec 2015
 */

#include <fcntl.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wav.h"
#include "wavr.h"
#include "signal.h"

#define WAVR_VERSION "0.4.4"

int main(int argc, char *argv[]) {
	struct signal_spec sigspec = {DEFAULT_SAMPLE_RATE,
		DEFAULT_SIGNAL_FREQUENCY,
		DEFAULT_SIGNAL_DURATION};

	struct wavr_args args;
	/* args.in_filename only initialized if necessary */
	args.out_filename = DEFAULT_OUTFILE;
	args.sample_dump = 0;
	args.thread_count = 1;
	args.input = INPUT_NONE; /* generate samples by default */
	args.sigspec = &sigspec;

	struct WavFile *wav;

	handle_args(&args, argc, argv);

	int oldstdout = dup(1);
	if (args.sample_dump) {
		/* exclusively output samples -> suppress all other output */
		freopen("/dev/null", "w", stdout);
	}

	printf("WAVr v%s\n", WAVR_VERSION);

	switch (args.input) {
		case INPUT_NONE:
			/* No input -> generate samples */
			wav = init_wav_file(&sigspec);

			printf("\nGenerating samples (%i thread(s))...\n", args.thread_count);
			wav->data = gen_sig(&sigspec, samplegen_sine, args.thread_count);
			
			printf("Finished sample generation.\n");
			break;
		case INPUT_STDIN:
			/* Parse samples from stdin */
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
		/* restore stdout */
		fclose(stdout);
		stdout = fdopen(oldstdout, "w");

		sample_dump(wav->data, &sigspec);	
	}

	/* cleanup */
	free_wavfile(wav);
	pthread_exit(NULL);
}

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
	exit(0);
}

void help(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);

	printf("Flags: \n"
		"-o <out_file>\t\tOutput wav to <out_file>\n"
		"-d\t\t\tDump samples to standard out (skip wav formatting)\n"
		"-t <duration>\t\tSet signal length to <duration> (s)\n"
		"-f <frequency>\t\tSet signal frequency to <frequency> Hz\n"
		"-s <sampleRate>\t\tSet signal sample rate to <sampleRate> Hz\n"
		"-l\t\t\tList available sample rates\n"
		"-i <in_file>\t\tInput wav from <in_file>\n"
		"-c\t\t\tInput samples from standard input\n"
		"-j <number>\t\tChange generation thread count\n"
		"-h\t\t\tWhat you just did\n");

	exit(0);
}

void handle_args(struct wavr_args *args, int argc, char *argv[]) {
	int c;
	extern int optind, optopt;

	while ((c = getopt(argc, argv, "o:dt:f:ls:i:cj:h")) != -1) {
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

			/* specify input file */
			case 'i':
				args->input = INPUT_FILE;
				args->in_filename = optarg;
				break;

			/* specify thread count */
			case 'j':
				args->thread_count = atoi(optarg);
				if (args->thread_count < 1) {
					printf("Invalid thread count: %i\n", args->thread_count);
					exit(1);
				}
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