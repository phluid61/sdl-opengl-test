/**
 * A simple, universal clock.
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include <stdint.h>
#include "qol.h"

typedef uint64_t nanosecond_t;
#define NANOSECOND_C(...) __CALL__(UINT64_C,__VA_CAT__(__VA_ARGS__))

nanosecond_t time_now();

#endif

/* vim: ts=4 sts=4 sw=4
 */
