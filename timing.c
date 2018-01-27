/*
 * Timing.
 *
 * A universal clock.
 */

#include "timing.h"

#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>

nanosecond_t time_now() {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		fprintf(stderr, "unable to check realtime clock\n");
		exit(-2);
	}
	return ((nanosecond_t)(ts.tv_sec) * UINT64_C(1000000000)) + (nanosecond_t)(ts.tv_nsec);
}

/* vim: ts=4 sts=4 sw=4
 */
