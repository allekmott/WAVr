/* wav.h
 * Wav structs & shit
 * Created: 20 Dec 2015
 */

#include <stdio.h>
#include <stdint.h>

#ifndef WAV_H
#define WAV_H

struct WavHeader {
	char ChunkID[4];
	uint32_t ChunkSize;
	char RIFFType[4];
};

struct FormatHeader {
	char ChunkID[4];
	uint32_t ChunkSize;
	uint16_t CompressionCode;
	uint16_t Channels;
	uint32_t SampleRate;
	uint32_t AvgBytesPerSec;
	uint16_t BlockAlign;
	uint16_t SigBitsPerSamp;
};

struct DataHeader {
	char ChunkID[4];
	uint32_t ChunkSize;
};

struct signal_spec {
	int sample_rate;
	float frequency;
	float duration;
};

FILE *init_wav_file(char *filename, struct signal_spec *sigspec);

#endif