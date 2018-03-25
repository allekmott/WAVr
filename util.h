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

/* float seconds -> long nanoseconds */
#define time_stous(n_time_s) (round((n_time_s) * 1e6))
#define time_ustos(n_time_us) ((n_time_us) * 1e-6)

/* parse string into floating point frequency value
 * returns -1.0 on failure */
float str_to_freq(const char *s_freq);

/* parse string time (seconds) into numeric time (microseconds)
 * returns 0 on failure */
unsigned long str_to_us_time(const char *s_time);

/* convert numeric time (microseconds) into string time (seconds) */
char *us_time_to_str(unsigned long n_time, char *s_time);

/* print warning message */
#define lame(args...) fprintf(stderr, args)

/* print error message & exit w/ errno */
#define super_lame(args...) { fprintf(stderr, args); exit(errno); }

/* print error message & errno description & exit w/ errno */
#define lame_error(msg) fprintf(stderr, "%s: %s\n", (msg), strerror(errno));

#endif /* __WAVR_UTIL_H__ */

