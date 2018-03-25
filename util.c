/**
 * @file	util.c
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Implementation of simple utility functions
 */

#include "util.h"

unsigned long str_to_us_time(const char *s_time) {
	float n_time_s;

	if (sscanf(s_time, "%f", &n_time_s) != 1)
	return 0;

	return time_stous(n_time_s);
}

char *us_time_to_str(unsigned long n_time_s, char *s_time) {
	sprintf(s_time, "%.2f", time_ustos(n_time_s));
	return s_time;
}

float str_to_freq(const char *s_freq) {
	float n_freq;

	if (sscanf(s_freq, "%f", &n_freq) != 1)
	n_freq = -1.0f;

	return n_freq;
}