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
