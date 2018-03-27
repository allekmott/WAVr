/**
 * @file	util.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Implementation of simple utility functions
 */

#include "util.h"

unsigned long str_to_time_us(const char *s_time) {
	float n_time_s;

	if (sscanf(s_time, "%f", &n_time_s) != 1)
		return 0;

	return time_s_to_us(n_time_s);
}

unsigned int str_to_freq_mhz(const char *s_freq) {
	float n_freq_hz;

	if (sscanf(s_freq, "%f", &n_freq_hz) != 1)
		return 0;

	return freq_hz_to_mhz(n_freq_hz);
}
