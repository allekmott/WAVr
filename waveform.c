/**
 * @file	waveform.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	15 April 2018
 * @brief	Implementations of basic waveform functions
 */

#include <stdio.h>
#include <string.h>

#include <math.h>
#include <float.h>
#include <limits.h>

#include "waveform.h"

enum waveform str_to_waveform(const char *s_waveform) {
	int n_waveform;

	if (sscanf(s_waveform, "%i", &n_waveform)) {
		/* already provided numeric value */
		if (n_waveform >= WAVEFORM_START_VAL && n_waveform <= WAVEFORM_END_VAL)
			return n_waveform;
		else
			return WAVEFORM_INVAL;
	} else {
		/* not provided numeric value, compare strings */
		for (n_waveform = WAVEFORM_START_VAL;
				n_waveform <= WAVEFORM_END_VAL;
				n_waveform++) {
			if (!strcmp(waveform_name(n_waveform), s_waveform))
				return n_waveform;
		}

		/* didn't match anything */
		return WAVEFORM_INVAL;
	}
}

double wave_func_sine(double t) {
	return sin(2.0 * M_PI * t);
}

double wave_func_square(double t) {
	return ((int) (2.0 * t) % 2 == 0) ? 1.0 : -1.0;
}

double wave_func_triangle(double t) {
	double t_mod1, t_adj;

	t_adj = 4.0 * t;
	t_mod1 = fmod(t_adj, 1.0);

	switch (((int) t_adj) % 4) {
		case 0:
			/* first quarter, on the up from 0 */
			return t_mod1;
		case 1:
			/* second quarter, on the down from 1 */
			return 1.0 - t_mod1;
		case 2:
			/* third quarter, on the down from 0 */
			return (-t_mod1);
		case 3:
			/* fourth quarter, on the up from -1 */
			return -1.0 + t_mod1;
	}

	return 0.0;
}

double wave_func_sawtooth(double t) {
	return fmod((2.0 * t), 2.0) - 1.0;
}

double wave_func_tangent(double t) {
	double val;

	/* FIXME: pretty sure this isn't correct cycle-wise */
	val = tan(M_PI * t) / (2 * M_PI);

	if (val > 1.0)
		return 1.0;
	else if (val < -1.0)
		return -1.0;
	else
		return val;
}
