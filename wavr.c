/* wavr.c
 * Main thingy
 * 5p4wn wav file
 * Created: 20 Dec 2015
 */

#include <errno.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wav.h"
#include "wavr.h"

int main(int argc, char *argv[]) {
	struct wav_file *file;

	char flag, s_time[128];
	int optind, optopt;

	char dump_samples;

	char *output_path;
	enum sample_rate sample_rate;
	unsigned long duration, s_duration;
	float frequency;

	/* defaults */
	output_path = "wavr_out.wav";
	sample_rate = SAMPLE_RATE_44100;	/* default to 44.1 kHz */
	duration = 1e6;						/* 1000000 us = 1s */
	frequency = 1e3f;					/* 1000 Hz */

	/* parse args */
	while ((flag = getopt(argc, argv, "o:pd:f:ls:i:cj:w:h")) != -1) {
		switch (flag) {
			case 'o': output_path = optarg; break;
			case 'p': /* TODO: dump samples */ break;
			case 'd':
				if (!(duration = str_to_us_time(optarg))) {
					lame("Invalid duration value: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'f':
				if (!(sscanf(optarg, "%f", &frequency))) {
					lame("Invalid frequency value: %s\n", optarg);
					return EINVAL;
				}
				break;
			case 'l': /* TODO: reimplement this */ break;
			case 's': /* TODO: reimplement this */ break;
			case 'i': output_path = optarg; break;
			case 'c': /* TODO: reimplement this */
			case 'j': /* TODO: reimplement this */
			case 'w': /* TODO: reimplement this */
			case 'h': printf("%s\n", WAVR_HELP_MSG); return 0;
		}
	}

	printf("WAVr v%s\n\n", WAVR_VERSION);

	printf("Output file: %s\nDuration:    %ss\nFrequency:   %.2f Hz\n",
			output_path, us_time_to_str(duration, s_time), frequency);

	printf("\nGenerating samples...\n");
	printf("Finished sample generation.\n");

	return 0;
}

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
	exit(0);
}

unsigned long str_to_us_time(const char *s_time) {
	float n_time_s;

	if (sscanf(s_time, "%f", &n_time_s) != 1)
		return 0;

	return time_stous(n_time_s);
}

char *us_time_to_str(unsigned long n_time_s, char *s_time) {
	sprintf(s_time, "%.2f", time_ustos(n_time_s));
	return s_time;
}

float str_to_freq(const char *s_freq) {
	float n_freq;

	if (sscanf(s_freq, "%f", &n_freq) != 1)
		n_freq = -1.0f;

	return n_freq;
}
