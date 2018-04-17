/**
 * @file	signal.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	26 March 2018
 * @brief	Definitions of signal-related data types & operations
 */

#ifndef __WAVR_SIGNAL_H__
#define __WAVR_SIGNAL_H__

/* 24-bit int is awkward, so we'll need this */
#define WAVR_INT24_MAX	(0x007fffff)

enum sample_bit_depth {
	SAMPLE_BIT_DEPTH_INVAL		= 0,
	SAMPLE_BIT_DEPTH_8			= 8,
	SAMPLE_BIT_DEPTH_16			= 16,
	SAMPLE_BIT_DEPTH_24			= 24,
	SAMPLE_BIT_DEPTH_32			= 32,
	SAMPLE_BIT_DEPTH_START_VAL	= SAMPLE_BIT_DEPTH_8,
	SAMPLE_BIT_DEPTH_END_VAL	= SAMPLE_BIT_DEPTH_32
};

enum sample_rate {
	SAMPLE_RATE_INVAL	= 0,
	SAMPLE_RATE_32K		= 32000,
	SAMPLE_RATE_44_1K	= 44100,
	SAMPLE_RATE_48K		= 48000,
	SAMPLE_RATE_88_2K	= 88200,
	SAMPLE_RATE_96K		= 96000,
	SAMPLE_RATE_192K	= 192000
};

/* parse sample rate from string */
enum sample_rate str_to_sample_rate(const char *s_sample_rate);

/* parse sample bit depth from string */
enum sample_bit_depth str_to_bit_depth(const char *s_sample_bit_depth);

/* parse amplitude from string */
float str_to_amplitude(const char *s_amplitude);

struct sample_format {
	enum sample_bit_depth	bit_depth;		/* bits per sample */
	enum sample_rate		sample_rate;	/* signal sample rate */
};

/* Description of a signal */
struct signal_desc {
	unsigned int			frequency;	/* signal frequency (mHz) */
	float					amplitude;	/* signal amplitude */
	unsigned long			duration;	/* signal duration (us) */
	struct sample_format	format;		/* sample format parameters */
};

#define signal_bytes_per_sample(sigp)	((sigp)->format.bit_depth / 8)

/* duration in microseconds, so conversion necessary */
#define signal_n_samples(sigp)			\
		(((sigp)->format.sample_rate) * ((sigp)->duration) * 1e-6)

/* calculate signal size in bytes */
#define signal_size_bytes(sigp) \
		((unsigned long) (((sigp)->duration)	*	\
			((sigp)->format.sample_rate)		* 	\
			signal_bytes_per_sample((sigp))		*	\
			1.0e-6))

/* render samples from double precision float to signal-specific format
 * @sig		Description of target signal
 * @buf		Array of raw samples (double precision float).
 			Also location to write to
 * @count	Number of samples contained within array
 */
int render_samples(struct signal_desc *sig, double *buf, unsigned int count);

/* dump samples to stdout */
void dump_samples(void *samples, unsigned int count,
		enum sample_bit_depth bit_depth);

#endif /* __WAVR_SIGNAL_H__ */
