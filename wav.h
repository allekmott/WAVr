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

/* pool everything together into singular struct;
 * make reading wav's in easy
 */
struct WavFile {
	struct WavHeader *wavHeader;
	struct FormatHeader *formatHeader;
	struct DataHeader *dataHeader;
	short *data;
};

struct WavFile *empty_wavfile();
void free_wavfile(struct WavFile *wav);

struct WavFile *init_wav_file(struct signal_spec *sigspec);
void write_wav_file(char *filename, struct WavFile *wav);

struct WavFile *read_wav_file(char *filename);
void sigspec_from_wav(struct WavFile *wav, struct signal_spec *sigspec);

#endif