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

#define WAVR_VERSION "0.1.2"

void usage(const char *cmd) {
	printf("Usage: %s <args>\n", cmd);
	exit(0);
}

int main(int argc, char *argv[]) {
	printf("WAVr v%s\n", WAVR_VERSION);

	int c;
	char *out_filename = "lol2.wav";
	extern int optind, optopt;

	int sampleDump = 0;

	int sampleRate = 41000;
	float frequency = 1000.0f;
	float duration = 1.0f;

	while ((c = getopt(argc, argv, "o:dt:f:ls:")) != -1) {
		switch (c) {
			/* set output filename */
			case 'o': out_filename = optarg; break;
			
			/* dump samples after generation */
			case 'd': sampleDump = 1; break;

			/* set duration of waveform */
			case 't':
				duration = atof(optarg);
				if (duration <= 0.0f)
					usage(argv[0]);
				break;

			/* set frequency of waveform */	
			case 'f': 
				frequency = atof(optarg);
				if (frequency <= 0.0f)
					usage(argv[0]);
				break;

			/* set waveform sample rate */
			case 's':
				sampleRate = atoi(optarg);
				if (sampleRate == 44100 ||
					sampleRate == 48000 ||
					sampleRate == 88200 ||
					sampleRate == 96000)
						break;
				else
					printf("Invalid sample rate: %i Hz\n", sampleRate);

			/* list available sample rates */
			case 'l':
				printf("Available sample rates:\n\n"
					"44,100 Hz (44.1 kHz)\n"
					"48,000 Hz (48.0 kHz)\n"
					"88,200 Hz (88.2 kHz)\n"
					"96,000 Hz (96.0 kHz)\n");
				exit(0);
				break;

			/* wtf */
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
	size_t dataSize = bytesize_gen(duration, sampleRate);

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
	formatHeader->SampleRate = (uint32_t) sampleRate; /* lol */
	formatHeader->SigBitsPerSamp = 16;
	formatHeader->BlockAlign = 2;
	formatHeader->AvgBytesPerSec = formatHeader->BlockAlign * sampleRate;

	/* data-specific stuff */
	strcpy(dataHeader->ChunkID, "data");
	dataHeader->ChunkSize = dataSize; /* lol */

	/* generate sample chain */
	printf("Generating samples...\n");
	short *samples = fap_sig(frequency, duration, sampleRate);
	printf("Finished sample generation.\n");

	printf("Writing to %s...\n", out_filename);
	FILE *out_file = fopen(out_filename, "w");
	fwrite(headerSpace, fullHeaderSize, 1, out_file);
	fwrite(samples, dataSize, 1, out_file);

	if (!sampleDump)
		printf("Write complete, exiting.\n");
	else {
		int numSamples = duration * sampleRate;
		int curSample;
		for (curSample = 0; curSample < numSamples; curSample++) {
			printf("0x%x\t", samples[curSample]);
			if (curSample % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}

	fclose(out_file);
	free(samples);
	free(headerSpace);
}
