/* wav.c
 * Specifies procedures to handle WAV
 * file formatting
 * Created: 15 Jan 2016
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>

#include "wav.h"

static int wav_file_read_header(struct wav_file *file);
static int wav_file_init_header(struct wav_file *file);
static int wav_file_write_header(struct wav_file *file);

static const mode_t WAV_FILE_MODE =
		S_IRUSR		/* read for user */
		| S_IWUSR	/* write for user */
		| S_IRGRP	/* read for group */
		| S_IWGRP	/* write for group */
		| S_IROTH;	/* read for other users */

struct wav_file *wav_file_open(const char *path, int create_if_dne) {
	struct wav_file *file;
	
	int fd;
	int flags;

	flags = O_RDWR; /* | O_APPEND; */
	if (create_if_dne)
		flags |= O_CREAT | O_EXCL;

	if ((fd = open(path, flags, WAV_FILE_MODE)) < 0) {
		if (errno == EEXIST) {
			/* file already exists. remove existing & create new one */
			unlink(path);

			/* retry open */
			if ((fd = open(path, flags, WAV_FILE_MODE)) < 0)
				return NULL;
		} else {
			return NULL;
		}
	}

	if (!(file = calloc(1, sizeof(struct wav_file))))
		return NULL;

	file->fd = fd;
	if (wav_file_init_header(file) < 0) {
		close(file->fd);
		return NULL;
	}

	return file;
}

/*
 * Read in header from wav file.
 * @file	Target wav file object
 *
 * Returns 0 on success, -1 on failure (errno set)
 */
static int wav_file_read_header(struct wav_file *file) {
	struct wav_header *header;

	int n_total_read, n_read;

	if (file == NULL) {
		errno = EBADF;
		return -1;
	}

	header = &file->header;

	n_total_read = 0;
	while (n_total_read < WAV_HEADER_SIZE) {
		n_read = read(file->fd,
				(header + n_total_read),
				(WAV_HEADER_SIZE - n_total_read));

		if (n_read < 0) {
			/* read error */
			return -1;
		} else if (n_read == 0) {
			/* insufficient file content to be header */
			errno = EINVAL;
			return -1;
		}

		n_total_read += n_read;
	}

	if (!wav_is_valid_header(header)) {
		errno = EINVAL;
		return -1;
	}

	return 0;
}

/* Write header to wav file
 * @file	Target wav file object
 *
 * Returns 0 on success, -1 on failure (errno set)
 */
static int wav_file_write_header(struct wav_file *file) {
	int n_total_written, n_written;

	if (file == NULL) {
		errno = EBADF;
		return -1;
	}

	/* move to beginning of file */
	if (lseek(file->fd, 0, SEEK_SET) < 0)
		return -1;

	n_total_written = 0;
	while (n_total_written < WAV_HEADER_SIZE) {
		n_written = write(file->fd,
				((&file->header) + n_total_written),
				(WAV_HEADER_SIZE - n_total_written));

		if (n_written < 1)
			/* write error */
			return -1;

		n_total_written += n_written;
	}

	return 0;
}

/*
 * Populate wav wav file's header with defaults & write to file
 * @file	Target wav file object
 *
 * Returns 0 on success, -1 on failure (errno set)
 */
static int wav_file_init_header(struct wav_file *file) {
	struct wav_header *header;

	if (file == NULL) {
		errno = EBADF;
		return -1;
	}

	header = &file->header;

	/* copy in default values */
	memset(header, 0, sizeof(struct wav_header));
	memcpy(&header->base_chunk, &BARE_RIFF_BASE_CHUNK,
			sizeof(struct riff_base_chunk));
	memcpy(&header->format_chunk, &BARE_WAV_FORMAT_CHUNK,
			sizeof(struct wav_format_chunk));
	memcpy(&header->data_chunk, &BARE_WAV_DATA_CHUNK,
			sizeof(struct wav_data_chunk));

	return wav_file_write_header(file);
}

int wav_file_close(struct wav_file *file) {
	int temp_fd;

	if (!file) {
		errno = -EBADF;
		return 0;
	}

	temp_fd = file->fd;
	free(file);
	
	return close(file->fd);
}

int wav_file_write_samples(struct wav_file *file,
		void *data, unsigned int count, unsigned int sample_offset) {
	struct wav_header *header;

	off_t start_offset, data_offset;
	unsigned int n_to_write, n_total_written;

	int n_written, err;

	if (file == NULL) {
		errno = EBADF;
		return -1;
	} else if (count == 0) {
		return 0;
	}

	header = &file->header;
	if (sample_offset > wav_file_n_samples(file)) {
		/* offset lies beyond current data */
		errno = EFAULT;
		return -1;
	}

	data_offset		= sample_offset * wav_file_sample_size(file);
	start_offset	= WAV_HEADER_SIZE + data_offset;
	n_to_write		= count * wav_file_sample_size(file);

	/* move seek position to write offset () */
	if (lseek(file->fd, start_offset, SEEK_SET) < 0)
		return -1;

	n_total_written = 0;
	while (n_total_written < n_to_write) {
		n_written = write(file->fd,
				(data + n_total_written),
				(n_to_write - n_total_written));

		if (n_written < 0) {
			err = errno;
			goto done;
		}

		n_total_written += n_written;
	}

	err = 0;

done:
	wav_file_set_data_size(file, (data_offset + n_total_written));

	if (err) {
		wav_file_write_header(file);

		errno = err;
		return -1;
	} else {
		if (wav_file_write_header(file) < 0)
			return -1;
		else
			return 0;
	}
}

