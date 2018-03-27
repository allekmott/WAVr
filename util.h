/**
 * @file	util.h
 * @author	Allek Mott <allekmott@gmail.com>
 * @date	25 March 2018
 * @brief	Utility functions & macros to simplify calculations/conversions
 */

#ifndef __WAVR_UTIL_H__
#define __WAVR_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/* wrpap memset in bzero interface */
#define coolzero(buf, size) memset((buf), 0, (size))

/* float seconds -> long nanoseconds */
#define time_s_to_us(n_time_s) (round((n_time_s) * 1e6))
#define time_us_to_s(n_time_us) ((n_time_us) * 1e-6)

/* parse string time (seconds) into numeric time (microseconds)
 * returns 0 on failure */
unsigned long str_to_time_us(const char *s_time);

/* float hz to unsigned int mhz */
#define freq_hz_to_mhz(n_freq_hz) ((unsigned int) ((n_freq_hz) * 1.0e3))

/* unsigned int mhz to float hz */
#define freq_mhz_to_hz(n_freq_mhz) (((float) (n_freq_mhz)) * 1.0e-3)

/* parse string into integer frequency value (milihertz)
 * returns 0 on failure */
unsigned int str_to_freq_mhz(const char *s_freq);

/* print warning message */
#define lame(args...) fprintf(stderr, args)

/* print error message & exit w/ errno */
#define super_lame(args...) { fprintf(stderr, args); exit(errno); }

/* print error message & errno description & exit w/ errno */
#define lame_error(msg) fprintf(stderr, "%s: %s\n", (msg), strerror(errno));

#endif /* __WAVR_UTIL_H__ */
