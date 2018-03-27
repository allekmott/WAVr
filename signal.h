/**
 * @file	signal.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	26 March 2018
 * @brief	Definitions of signal-related data types & operations
 */

#ifndef __WAVR_SIGNAL_H__
#define __WAVR_SIGNAL_H__

#include <limits.h>

/*
 * Signal object
 *
 * All signals are initially be generated synthesised into doubles. The are
 * then rendered into the file's specified audio format before writeout.
 */
struct wavr_signal {
	unsigned int	frequency;		/* signal frequency (mHz) */
	float			amplitude;		/* signal amplitude */
	unsigned long	duration;		/* signal duration (us) */
	double			*samples;		/* pointer to samples */
	unsigned long	n_samples;		/* sample count */
	unsigned int	sample_rate;	/* sample rate enumeration */
};

enum wavr_sample_type {
	WAVR_SAMPLE_TYPE_8_BIT,
	WAVR_SAMPLE_TYPE_16_BIT,
	WAVR_SAMPLE_TYPE_24_BIT,
	WAVR_SAMPLE_TYPE_32_BIT
};
static const unsigned char wavr_sample_type_bit_depth[] = { 8, 16, 24, 32 };

/* mapping from sample type -> maximum sample value */
static const unsigned int wavr_sample_type_max_value[] = {
	CHAR_MAX,		/* 8 bit */
	SHRT_MAX,		/* 16 bit */
	INT_MAX >> 8,	/* 24 bit (32bit max, but without high byte) */
	1				/* 32 bit (float) */
};

/* mapping from sample type -> minimum sample value */
static const unsigned int wavr_sample_type_min_value[] = {
	0,					/* 8 bit */
	SHRT_MIN,			/* 16 bit */
	~(INT_MAX >> 8),	/* 24 bit (complement of max) */
	-1					/* 32 bit (float) */
};

/* cheeky macros  */
#define sample_type_bits(type) (sample_type_bit_depth[(type)])
#define sample_type_min(type) (sample_type_min_value[(type)])
#define sample_type_max(type) (sample_type_max_value[(type)])

#endif /* __WAVR_SIGNAL_H__ */
