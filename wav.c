/* wav.c
 * Specifies procedures to handle WAV
 * file formatting
 * Created: 15 Jan 2016
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"
#include "signal.h"

struct WavFile *empty_wavfile() {
	struct WavFile *wav = malloc(sizeof(struct WavFile));
	wav->wavHeader = malloc(sizeof(struct WavHeader));
	wav->formatHeader = malloc(sizeof(struct FormatHeader));
	wav->dataHeader = malloc(sizeof(struct DataHeader));

	return wav;
}

void free_wavfile(struct WavFile *wav) {
	free(wav->wavHeader);
	free(wav->formatHeader);
	free(wav->dataHeader);
	free(wav->data);
	free(wav);
}

struct WavFile *init_wav_file(struct signal_spec *sigspec) {
	/* grab an empty struct */
	struct WavFile *wav = empty_wavfile();

	/* fill out header info stuff */
	strncpy(wav->wavHeader->ChunkID, "RIFF", 4);
	wav->wavHeader->ChunkSize = (sizeof(struct WavHeader));
	strncpy(wav->wavHeader->RIFFType, "WAVE", 4);

	strncpy(wav->formatHeader->ChunkID, "fmt ", 4);
	wav->formatHeader->ChunkSize = 16;
	wav->formatHeader->CompressionCode = 1;
	wav->formatHeader->Channels = 1;
	wav->formatHeader->SampleRate = (uint32_t) sigspec->sample_rate; /* lol */
	wav->formatHeader->SigBitsPerSamp = 16;
	wav->formatHeader->BlockAlign = 2;
	wav->formatHeader->AvgBytesPerSec = wav->formatHeader->BlockAlign * sigspec->sample_rate;

	strncpy(wav->dataHeader->ChunkID, "data", 4);
	wav->dataHeader->ChunkSize = bytesize_gen(sigspec);
	return wav;
}

void write_wav_file(char *filename, struct WavFile *wav) {
	/* set up clock_t's and grab start time */
	clock_t start, diff;
	start = clock();

	/* openy openy */
	FILE *out = fopen(filename, "wb");
	if (out == NULL) {
		fprintf(stderr, "Unable to open file '%s' for writing\n", filename);
		exit(1);
	}
	printf("Writing to '%s'\n", filename);

	/* ... and thats really all that has to be done before writing. */
	fwrite(wav->wavHeader, sizeof(struct WavHeader), 1, out);
	fwrite(wav->formatHeader, sizeof(struct FormatHeader), 1, out);
	fwrite(wav->dataHeader, sizeof(struct DataHeader), 1, out);

	/* data header's ChunkSize is literally the size of the sample array */
	fwrite(wav->data, wav->dataHeader->ChunkSize, 1, out);

	/* calculated elasped time */
	diff = clock() - start;
	float elapsed = (float) diff / (float) CLOCKS_PER_SEC;

	printf("Write complete (%.2fs)\n", elapsed);
	fclose(out);
}

struct WavFile *read_wav_file(char *filename) {
	/* aight, let's try to open this dude */
	FILE *in = fopen(filename, "rb");
	if (in == NULL) {
		fprintf(stderr, "Unable to open file '%s' for reading\n", filename);
		exit(1);
	}

	//printf("\nReading file '%s'\n", filename);

	/* file open, allocate room for structs */
	struct WavFile *wav = empty_wavfile();

	/* read in headers so we know how many samples we're looking at */
	fread(wav->wavHeader, sizeof(struct WavHeader), 1, in); /* Wav Header */
	fread(wav->formatHeader, sizeof(struct FormatHeader), 1, in); /* Format Header */
	fread(wav->dataHeader, sizeof(struct DataHeader), 1, in); /* Data Header */

	size_t sizeToRead = wav->dataHeader->ChunkSize;
	/*printf("%d bytes to be read / %ikHz -> %.2fs\n", wav->dataHeader->ChunkSize,
			wav->formatHeader->SampleRate / 1000,
			(float) wav->dataHeader->ChunkSize / (float) wav->formatHeader->SampleRate);*/
	
	/* most problematic allocation, so we'll just make sure here... */
	short *data = malloc(sizeToRead);
	if (data == NULL) {
		fprintf(stderr, "Unable to allocate %d bytes for data segment...\n", (int) sizeToRead);
		free_wavfile(wav);
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