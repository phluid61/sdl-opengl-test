/**
 * A simple, universal clock.
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include <stdint.h>

typedef uint64_t nanosecond_t;
#define NANOSECOND_C(v) UINT64_C(v)

nanosecond_t time_now();

#endif

/* vim: ts=4 sts=4 sw=4
 */
