/* wav.h
 * Wav structs & shit
 * Created: 20 Dec 2015
 */

#include <stdint.h>

#ifndef _WAV_H
#define _WAV_H

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

#endif