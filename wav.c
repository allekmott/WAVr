/* wav.c
 * Specifies procedures to handle WAV
 * file formatting
 * Created: 15 Jan 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"
#include "signal.h"

FILE *init_wav_file(char *filename, struct signal_spec *sigspec) {
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
	size_t dataSize = bytesize_gen(sigspec);

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
	formatHeader->SampleRate = (uint32_t) sigspec->sample_rate; /* lol */
	formatHeader->SigBitsPerSamp = 16;
	formatHeader->BlockAlign = 2;
	formatHeader->AvgBytesPerSec = formatHeader->BlockAlign * sigspec->sample_rate;

	/* data-specific stuff */
	strcpy(dataHeader->ChunkID, "data");
	dataHeader->ChunkSize = dataSize; /* lol */

	FILE *wav_file = fopen(filename, "w");
	fwrite(headerSpace, fullHeaderSize, 1, wav_file);

	free(headerSpace);

	return wav_file;
}

struct WavFile *read_wav_file(char *filename) {
	/* aight, let's try to open this dude */
	FILE *in = fopen(filename, "rb");
	if (in == NULL) {
		fprintf(stderr, "Unable to open file %s for reading\n", filename);
		exit(1);
	}

	printf("\nReading file '%s'\n", filename);

	/* file open, allocate room for structs */
	struct WavFile *wav = malloc(sizeof(struct WavFile));
	struct WavHeader *wavHeader = malloc(sizeof(struct WavHeader));
	struct FormatHeader *formatHeader = malloc(sizeof(struct FormatHeader));
	struct DataHeader *dataHeader = malloc(sizeof(struct DataHeader));

	wav->wavHeader = wavHeader;
	wav->formatHeader = formatHeader;
	wav->dataHeader = dataHeader;

	/* read in headers so we know how many samples we're looking at */
	fread(wavHeader, sizeof(struct WavHeader), 1, in); /* Wav Header */
	fread(formatHeader, sizeof(struct FormatHeader), 1, in); /* Format Header */
	fread(dataHeader, sizeof(struct DataHeader), 1, in); /* Data Header */

	size_t sizeToRead = dataHeader->ChunkSize;
	printf("%d bytes to be read / %ikHz -> %.2fs\n", wav->dataHeader->ChunkSize,
			wav->formatHeader->SampleRate / 1000,
			(float) wav->dataHeader->ChunkSize / (float) wav->formatHeader->SampleRate);
	
	/* most problematic allocation, so we'll just make sure here... */
	short *data = malloc(sizeToRead);
	if (data == NULL) {
		fprintf(stderr, "Unable to allocate %d bytes for data segment...\n", (int) sizeToRead);
		free(wavHeader);
		free(formatHeader);
		free(dataHeader);
		free(wav);

		exit(1);
	}

	fread(data, sizeToRead, 1, in); /* read dem samples in */
	wav->data = data; /* make sure wav actually gets his samples */

	fclose(in);

	/* all done! */
	return wav;
}

void sigspec_from_wav(struct WavFile *wav, struct signal_spec *sigspec) {
	sigspec->sample_rate = wav->formatHeader->SampleRate;
	sigspec->frequency = -1.0f;

	/* ah, duration. */
	float duration = (float) wav->dataHeader->ChunkSize / (float) wav->formatHeader->SampleRate;
	sigspec->duration = duration;
}