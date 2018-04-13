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

/* RIFF file format descriptor; this is basically a superchunk: all other
 * chunks contained in the file are part of this one */
struct riff_base_chunk {
	char chunk_id[4];			/* [big] chunk identifier ('RIFF') */
	unsigned int chunk_size;	/* [lit] size of this chunk */
	char riff_type[4];			/* [big] file format code  ('WAVE') */
};

/* Parameters for format of audio contained in WAV file */
struct wav_audio_format {
	unsigned short	compression_code;	/* [lit] audio format */
	unsigned short	n_channels;			/* [lit] number of channels */
	unsigned int	sample_rate;		/* [lit] sample rate, khz */
	unsigned int	byte_rate;			/* [lit] average bytes/s */
	unsigned short	block_align;		/* [lit] # bytes/sample * # channels */
	unsigned short	bit_depth;			/* [lit] bits/sample */
};

/* WAV-specific parameters describing the data */
struct wav_format_chunk {
	char chunk_id[4];					/* [big] chunk identifier ('fmt ') */
	unsigned int chunk_size;			/* [lit] size of subchunk (format) */
	struct wav_audio_format format;		/* audio format */
};

/* **PARTIAL** just top of data chunk; the rest of the chunk contains the
 * actual contents of the wav file */
struct wav_data_chunk {
	char chunk_id[4];			/* [big] chunk identifier ('data') */
	unsigned int chunk_size;	/* [lit] size of subchunk (actual data) */
};

/* entire header */
struct wav_header {
	struct riff_base_chunk	base_chunk;		/* RIFF header */
	struct wav_format_chunk	format_chunk;	/* Format chunk */
	struct wav_data_chunk	data_chunk;		/* Data chunk */
};

#define RIFF_CHUNK_ID	{ 'R', 'I', 'F', 'F' }
#define RIFF_TYPE_WAVE	{ 'W', 'A', 'V', 'E' }
static const struct riff_base_chunk BARE_RIFF_BASE_CHUNK = {
	.chunk_id	= RIFF_CHUNK_ID,			/* this is a RIFF header */
	.riff_type	= RIFF_TYPE_WAVE,			/* this is a WAV file */
	.chunk_size = sizeof(struct wav_header) /* empty wav -> size of header */
};

#define WAV_FORMAT_CHUNK_ID { 'f', 'm', 't', ' ' }
static const struct wav_format_chunk BARE_WAV_FORMAT_CHUNK = {
	.chunk_id	= WAV_FORMAT_CHUNK_ID,	/* format chunk */

	/* subchunk size, so size of the audio format following */
	.chunk_size	= sizeof(struct wav_audio_format),

	/* defaults */
	.format = {
		.compression_code	= 1,		/* linear quantisation */
		.n_channels			= 1,		/* mono */
		.sample_rate		= 44100,	/* 44.1 kHz */
		.byte_rate			= 88200,	/* 44.1 16-bit samples/s */
		.block_align		= 2,		/* # channels * bytes/sample */
		.bit_depth			= 16		/* 16-bit signed int  */
	}
};

#define WAV_DATA_CHUNK_ID { 'd', 'a', 't', 'a' }
static const struct wav_data_chunk BARE_WAV_DATA_CHUNK = {
	.chunk_id	= WAV_DATA_CHUNK_ID,	/* data chunk */
	.chunk_size = 0						/* no data yet */
};

/* simple wav file struct; header & contents */
struct wav_file {
	int fd;						/* file descriptor, < 0 if error condition */
	const char *path;			/* path to file */
	struct wav_header header;	/* file header */
};

/* some file property macros to enable laziness */
#define WAV_HEADER_SIZE			(sizeof(struct wav_header))

#define wav_is_valid_header(header)							\
		(((int) (header)->base_chunk.chunk_id) ==			\
					(int) ((char [4]) RIFF_CHUNK_ID)		\
			&& ((int) (header)->base_chunk.riff_type) ==	\
					(int) ((char [4]) RIFF_TYPE_WAVE)		\
			&& ((int) (header)->format_chunk.chunk_id) ==	\
					(int) ((char [4]) WAV_FORMAT_CHUNK_ID)	\
			&& ((int) (header)->data_chunk.chunk_id) ==		\
					(int) ((char [4]) WAV_DATA_CHUNK_ID))

#define wav_file_header(file)		(&(file)->header)
#define wav_file_data_chunk(file)	(&wav_file_header((file))->data_chunk)
#define wav_file_base_chunk(file)	(&wav_file_header((file))->base_chunk)
#define wav_file_format_chunk(file)	(&wav_file_header((file))->format_chunk)
#define wav_file_format(file)		(&wav_file_format_chunk((file))->format)

#define wav_file_set_format(file, fmt)	memcpy(wav_file_format((file)), fmt)

#define wav_file_n_channels(file)	(wav_file_format((file))->n_channels)
#define wav_file_sample_rate(file)	(wav_file_format((file))->sample_rate)
#define wav_file_bit_depth(file)	(wav_file_format((file))->bit_depth)
#define wav_file_sample_size(file)	(wav_file_bit_depth((file)) / 8)

#define wav_file_size(file)	(wav_file_base_chunk((file))->chunk_size)
#define wav_file_data_size(file)	(wav_file_data_chunk((file))->chunk_size)

#define wav_file_set_data_size(file, size) {								  \
			wav_file_base_chunk((file))->chunk_size = WAV_HEADER_SIZE + size; \
			wav_file_data_chunk((file))->chunk_size = size;					  \
		}

#define wav_file_n_samples(file)	\
		(wav_file_data_size((file)) / wav_file_sample_size((file)))

/*
 * Open existing wav file
 * @path			Path to target file
 * @create_if_dne	Create file if it doesn't exist
 *
 * Returns new wav_file object on success, NULL on failure (errno set)
 */
struct wav_file *wav_file_open(const char *path, int create_if_dne);

/*
 * Close wav file & free wav file object
 * @file	Target wav file
 *
 * Returns 0 on success, -1 on failure (errno set)
 */
int wav_file_close(struct wav_file *file);

/*
 * Read in samples from a wav file
 * @file			Target wav file object
 * @samples			Buffer to write samples to
 * @n_samples		Number of samples to read
 * @sample_offset	Offset from start of sample data to begin
 *
 * Returns # of samples read in, -1 on failure (errno set)
 */
int wav_file_read_samples(const struct wav_file *file,
		void *samples, int n_samples, unsigned int sample_offset);

/*
 * Write samples to the wav file. Also adjusts header to reflect changes.
 * @file			Wav file to be written to
 * @samples			Buffer containing samples to be written
 * @n_samples		Number of samples to be written
 * @sample_offset	Offset from start of sample data to begin
 *
 * Returns 0 on success, negative errno on error
 */
int wav_file_write_samples(struct wav_file *file,
		void *samples, unsigned int n_samples, unsigned int sample_offset);

#endif /* __WAVR_WAV_H__ */

