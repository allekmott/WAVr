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
#include "sigfapper.h"

#define WAVR_VERSION "0.0.9_w2"
#define WAV_SAMPLE_RATE 44100
#define WAV_DURATION 20

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
	exit(0);
}

int main(int argc, char *argv[]) {
	printf("WavR v%s\n", WAVR_VERSION);

	int c;
	char *out_filename = "lol2.wav";
	extern int optind, optopt;

	int sampleDump = 0;

	while ((c = getopt(argc, argv, "o:d")) != -1) {
		switch (c) {
			case 'o': out_filename = optarg; break;
			case 'd': sampleDump = 1; break;
			case '?': usage(argv[0]); break;
		}
	}

	int fullHeaderSize = sizeof(struct WavHeader)
		+ sizeof(struct FormatHeader)
		+ sizeof(struct DataHeader);

	struct WavHeader *wavHeader;
	struct FormatHeader *formatHeader;
	struct DataHeader *dataHeader;

	void *headerSpace = malloc(fullHeaderSize);
	wavHeader = (struct WavHeader *)headerSpace;
	formatHeader = (struct FormatHeader *)(wavHeader + 1);
	dataHeader = (struct DataHeader *)(formatHeader + 1);

	/* prospected wav data size */
	size_t dataSize = bytesize_gen(WAV_DURATION, WAV_SAMPLE_RATE);

	/* begin populating headers with values */

	/* wav-specific stuff */
	strcpy(wavHeader->ChunkID, "RIFF");
	wavHeader->ChunkSize = (uint32_t) fullHeaderSize - 8;
	strcpy(wavHeader->RIFFType, "WAVE");

	/* format-specific stuff */
	strcpy(formatHeader->ChunkID, "fmt ");
	formatHeader->ChunkSize = 16;
	formatHeader->CompressionCode = 1;
	formatHeader->Channels = 1;
	formatHeader->SampleRate = (uint32_t) WAV_SAMPLE_RATE; /* lol */
	formatHeader->SigBitsPerSamp = 16;
	formatHeader->BlockAlign = 2;
	formatHeader->AvgBytesPerSec = formatHeader->BlockAlign * WAV_SAMPLE_RATE;

	/* data-specific stuff */
	strcpy(dataHeader->ChunkID, "data");
	dataHeader->ChunkSize = dataSize; /* lol */

	/* generate sample chain */
	printf("Generating samples...\n");
	short *samples = fap_sig(1000, WAV_DURATION, WAV_SAMPLE_RATE);
	printf("Finished sample generation.\n");

	printf("Writing to %s...\n", out_filename);
	FILE *out_file = fopen(out_filename, "w");
	fwrite(headerSpace, fullHeaderSize, 1, out_file);
	fwrite(samples, dataSize, 1, out_file);

	if (!sampleDump)
		printf("Write complete, exiting.\n");
	else {

	}

	fclose(out_file);
	free(samples);
	free(headerSpace);
}