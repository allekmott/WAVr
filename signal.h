/**
 * @file	signal.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	26 March 2018
 * @brief	Definitions of signal-related data types & operations
 */

#ifndef __WAVR_SIGNAL_H__
#define __WAVR_SIGNAL_H__

enum sample_bit_depth {
	SAMPLE_BIT_DEPTH_INVAL	= 0,
	SAMPLE_BIT_DEPTH_8		= 8,
	SAMPLE_BIT_DEPTH_16		= 16,
	SAMPLE_BIT_DEPTH_24		= 24,
	SAMPLE_BIT_DEPTH_32		= 32
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

/* parse sample rate from string */
enum sample_rate str_to_sample_rate(const char *s_sample_rate);

/* parse sample bit depth from string */
enum sample_bit_depth str_to_bit_depth(const char *s_sample_bit_depth);

/* dump samples to stdout */
void dump_samples(void *samples, unsigned int count,
		enum sample_bit_depth bit_depth);

/* calculate signal size in bytes */
#define signal_size_bytes(sigp) \
		((unsigned long) (((sigp)->duration)	*	\
			((sigp)->format.sample_rate)		* 	\
			((sigp)->format.bit_depth / 8)		*	\
			1.0e-6))

/* some wave functions */
#define wave_sine(t)			sin((t) * (2.0 * M_PI))
double wave_square(double t);
double wave_triangle(double t);

#endif /* __WAVR_SIGNAL_H__ */
