/**
 * @file	waveform.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	15 April 2018
 * @brief	Declarations of waveform data structures and operations;
 *			Where the actual signal math happens
 */

#ifndef __WAVR_WAVEFORM_H__
#define __WAVR_WAVEFORM_H__

/* wave function; to input time (double), output result (double) */
typedef double (*wave_func_t) (double);

/* some wave functions */
double wave_func_sine(double t);
double wave_func_square(double t);
double wave_func_triangle(double t);
double wave_func_sawtooth(double t);

/* base waveforms */
enum waveform {
	WAVEFORM_INVAL		= 0,				/* [invalid] */
	WAVEFORM_SINE,							/* sine wave generator */
	WAVEFORM_SQUARE,						/* square wave generator */
	WAVEFORM_TRIANGLE,						/* triangle wave generator */
	WAVEFORM_SAWTOOTH,
	WAVEFORM_START_VAL	= WAVEFORM_SINE,
	WAVEFORM_END_VAL	= WAVEFORM_SAWTOOTH
};

static const char *WAVEFORM_NAMES[] = {
	"sine",
	"square",
	"triangle",
	"sawtooth"
};

static const wave_func_t WAVEFORM_FUNCS[] = {
	wave_func_sine,
	wave_func_square,
	wave_func_triangle,
	wave_func_sawtooth
};

#define waveform_name(waveform) WAVEFORM_NAMES[(waveform) - WAVEFORM_START_VAL]
#define waveform_func(waveform) WAVEFORM_FUNCS[(waveform) - WAVEFORM_START_VAL]

/* parse waveform enum value from string */
enum waveform str_to_waveform(const char *s_waveform);

#endif /* __WAVR_WAVEFORM_H__ */
