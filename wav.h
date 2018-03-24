/**
 * @file	wav.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	20 December 2015
 * @brief	Declarations of wav file structures & operations
 */

#ifndef __WAVR_WAV_H__
#define __WAVR_WAV_H__

#include <stdio.h>
#include <stdint.h>

/* Parameters for format of audio contained in WAV file */
struct wav_audio_format {
	unsigned short	compression_code;	/* [lit] audio format */
	unsigned short	n_channels;			/* [lit] number of channels */
	unsigned int	sample_rate;		/* [lit] sample rate, khz */
	unsigned int	byte_rate;			/* [lit] average bytes/s */
	unsigned short	block_align;		/* [lit] */
	unsigned short	bit_depth;			/* [lit] bits/sample */
};

/* RIFF file format descriptor; this is basically a superchunk: all other
 * chunks contained in the file are part of this one */
struct riff_base_chunk {
	char chunk_id[4];			/* [big] chunk identifier */
	unsigned int chunk_size;	/* [lit] size of this chunk */
	char riff_type[4];			/* [big] file format code */
};

/* WAV-specific parameters describing the data */
struct wav_format_chunk {
	char chunk_id[4];					/* [big] chunk identifier */
	unsigned int chunk_size;			/* [lit] size of this chunk */
	struct wav_audio_format format;		/* audio format */
};

/* **PARTIAL** just top of data chunk; the rest of the chunk contains the
 * actual contents of the wav file */
struct wav_data_chunk {
	char chunk_id[4];
	unsigned int chunk_size;
};

static const struct riff_base_chunk BARE_RIFF_BASE_CHUNK = {
	.chunk_id	= { 'R', 'I', 'F', 'F' },	/* this is a RIFF header */
	.riff_type	= { 'W', 'A', 'V', 'E' }	/* this is a WAV file */

	/* .chunk_size is directly dependent on file size, won't set until later */
};

static const struct wav_format_chunk BARE_WAV_FORMAT_CHUNK = {
	.chunk_id	= { 'f', 'm', 't', ' ' },	/* format chunk */

	/* size of this chunk is static, and its own size */
	.chunk_size	= sizeof(struct wav_format_chunk)

	/* .format will be provided later */
};

static const struct wav_data_chunk BARE_WAV_DATA_CHUNK = {
	.chunk_id	= { 'd', 'a', 't', 'a' },
	/* .chunk_size depends on size of contents that follow, save for later */
};

struct wav_header {
	struct riff_base_chunk	base_chunk;		/* RIFF header */
	struct wav_format_chunk	format_chunk;	/* Format chunk */
	struct wav_data_chunk	data_chunk;		/* Data chunk */
};

/* simple wav file struct; header & contents */
struct wav_file {
	const char *path;
	struct wav_header header;	/* file header */
	void *samples;					/* samples */
};

/* Write audio format info to header */
void wav_set_audio_format
(struct wav_file *file, struct wav_audio_format *format);

void wav_add_samples
(struct wav_file *file, void *samples, int *n_samples, int offset);

#define wav_append_samples(f, s, n) wav_add_samples(f, s, n, 0)

struct wav_file *wav_new_file(const char *path);
struct wav_file *wav_open_file(const char *path);
int wav_write_file(const struct wav_file *file);
void wav_free_file(struct wav_file *file);

int wav_write_samples(FILE *file, void *samples, size_t size, size_t offset);

#endif /* __WAVR_WAV_H__ */

