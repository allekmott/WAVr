/**
 * @file	generator.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	22 March 2018
 * @brief	Declarations of types & routines to be utilised in signal
 *			generation.
 */

#ifndef __WAVR_GENERATOR_H__
#define __WAVR_GENERATOR_H__

struct signal_params {
	unsigned int	sample_rate;	/* signal sample rate */
	float			frequency;		/* waveform frequency */
	unsigned long 	duration;		/* duration of signal in (us) */
	long			t_offset;		/* offset from t=0 to begin (us) */
	void			*buffer;		/* pointer to sample buffer */
};

/* Generates & renders samples using the provided generator function */
int generate_signal(
		unsigned int sample_rate, float frequency, unsigned long duration,
		void (*generator) (struct signal_params *));

/* some signal generators */
void sine_generator(struct signal_params *params);
void square_generator(struct signal_params *params);
void triangel_generator(struct signal_params *params);

#endif /* __WAVR_GENERATOR_H__ */
