/**
 * @file	signal.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	26 March 2018
 * @brief	Definitions of signal-related data types & operations
 */

#ifndef __WAVR_SIGNAL_H__
#define __WAVR_SIGNAL_H__

enum sample_bit_depth {
	SAMPLE_BIT_DEPTH_8	= 8,
	SAMPLE_BIT_DEPTH_16	= 16,
	SAMPLE_BIT_DEPTH_24	= 24,
	SAMPLE_BIT_DEPTH_32	= 32
};

enum sample_rate {
	SAMPLE_RATE_32K		= 32000,
	SAMPLE_RATE_44_1K	= 44100,
	SAMPLE_RATE_48K		= 48000,
	SAMPLE_RATE_88_2K	= 88200,
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

#endif /* __WAVR_SIGNAL_H__ */
