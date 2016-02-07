/* wavr.h
 * Declares argument struct and all argument
 * handling functions.
 * Created: 14 January 2016
 */

#include "wav.h"

#ifndef WAVR_H
#define WAVR_H

#define DEFAULT_SAMPLE_RATE 41000
#define DEFAULT_SIGNAL_FREQUENCY 1000.0f
#define DEFAULT_SIGNAL_DURATION 1.0f
#define DEFAULT_OUTFILE "wavrgen.wav"

struct wavr_args {
	char *out_filename;
	int sample_dump;
	int sample_stdin;	struct signal_spec *sigspec;
};

void usage(const char *cmd);
void help(const char *cmd);
void handle_args(struct wavr_args *args, int argc, char *argv[]);


#endif