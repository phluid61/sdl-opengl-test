/**
 * The Engine is (currently) singleton.  This file defines the
 * types and macros is uses, and exposes the public methods.
 */

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdint.h>

#include "timing.h"
#include "entity.h"

/* The time between ticks. */
extern const nanosecond_t Engine__tick_rate;

/**
 * Calling this method may result in a tick().
 * Ticks are never less than Engine__tick_rate apart.
 */
void Engine__maybe_tick();

/**
 * Registers a single Entity in the engine.
 */
void Engine__register_entity(Entity_t *entity);

#endif

/* vim: ts=4 sts=4 sw=4
 */
