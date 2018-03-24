/* wav.c
 * Specifies procedures to handle WAV
 * file formatting
 * Created: 15 Jan 2016
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wav.h"

static void wav_init_header(struct wav_header *header) {
	memset(header, 0, sizeof(struct wav_header));
	memcpy(&header->base_chunk, &BARE_RIFF_BASE_CHUNK,
			sizeof(struct riff_base_chunk));
	memcpy(&header->format_chunk, &BARE_WAV_FORMAT_CHUNK,
			sizeof(struct wav_format_chunk));
	memcpy(&header->data_chunk, &BARE_WAV_DATA_CHUNK,
			sizeof(struct wav_data_chunk));
}

void wav_set_audio_format
(struct wav_file *file, struct wav_audio_format *format) {
	memcpy(&file->header.format_chunk.format, format,
			sizeof(struct wav_audio_format));
}

void wav_add_samples
(struct wav_file *file, void *samples, int *n_samples, int offset) {

}

struct wav_file *wav_new_file(const char *path) {
	struct wav_file *file;

	if (!(file = calloc(1, sizeof(struct wav_file))))
		return NULL;

	file->path = path;

	/* new wav file -> bare header for now */
	wav_init_header(&file->header);

	return file;
}

struct wav_file *wav_open_file(const char *path) {
	struct wav_file *file;

	int fd;
	size_t n_total_read, n_read;

	if ((fd = open(path, O_RDONLY | O_EXCL)) == -1)
		return NULL;

	if (!(file = calloc(1, sizeof(struct wav_file)))) {
		close(fd);
		return NULL;
	}

	file->path = path;

	/* existing wav file -> attempt to read in header */
	n_total_read = 0;
	while (n_total_read < sizeof(struct wav_header)) {
		n_read = read(fd, &file->header, sizeof(struct wav_header));
		if (n_read < 1) {
			close(fd);
			free(file);

			return NULL;
		}

		n_total_read += n_read;
	}

	close(fd);
	return file;
}

int wav_save_file(const struct wav_file *file) {
	int fd;
	size_t n_total_written, n_written, data_size;

	if ((fd = open(file->path, O_WRONLY | O_CREAT)) == -1)
		return -1;

	/* write header */
	n_total_written = 0;
	while (n_total_written < sizeof(struct wav_header)) {
		n_written = write(fd, &file->header, sizeof(struct wav_header));
		if (n_written < 1)
			goto error;

		n_total_written += n_written;
	}

	/* write samples */
	data_size = file->header.data_chunk.chunk_size;

	n_total_written = 0;
	while (n_total_written < data_size) {
		n_written = write(fd, &file->samples, data_size);
		if (n_written < 1)
			goto error;

		n_total_written += n_written;
	}

	close(fd);
	return 0;

error:
	close(fd);
	return -1;
}

void wav_free_file(struct wav_file *file) {
	if (!file)
		return;

	if (file->samples)
		free(file->samples);

	free(file);
}
