/* wavr.c
 * Main thingy
 * 5p4wn wav file
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>

#include "wav.h"

#define WAVR_VERSION "0.0.3"

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
	exit(0);
}

int main(int argc, char *argv[]) {
	printf("WavR v%s\n", WAVR_VERSION);

	int c;
	char *out_file;
	extern int optind, optopt;

	while ((c = getopt(argc, argv, "o:")) != -1) {
		switch (c) {
			case 'o': out_file = optarg; break;
			case '?': usage(argv[0]); break;
		}
	}

}