/*
 * Engine.
 *
 * Synchronises entities, controls tick rate.
 */

#include "engine.h"
#include "entity.h"
#include "timing.h"

#include <stdlib.h>
#include <stdio.h>

#define NEVER (nanosecond_t)(~0)

typedef struct EntityList {
	Entity_t *e;
	struct EntityList *next;
} EntityList_t;

EntityList_t *ents = NULL;

static void add_entity(Entity_t *e) {
	EntityList_t *ptr;
	EntityList_t *node;
	node = malloc(sizeof(EntityList_t));
	node->e = e;
	node->next = NULL;

	if (ents == NULL) {
		ents = node;
	} else {
		ptr = ents;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = node;
	}
}
/* this frees the node, but not the entity */
static void rm_entity(Entity_t *e) {
	EntityList_t *next;
	EntityList_t *this;
	if (ents == NULL) {
		return;
	}
	if (ents->e == e) {
		next = ents;
		ents = ents->next;
		free(next);
		return;
	}
	this = ents;
	while (this != NULL) {
		next = this->next;
		if (next != NULL && next->e == e) {
			this->next = next->next;
			free(next);
			return;
		}
		this = next;
	}
}
static void each_entity(void (*func)(Entity_t*)) {
	EntityList_t *ptr = ents;
	while (ptr) {
		/*if (ptr->e)*/
			func(ptr->e);
		ptr = ptr->next;
	}
}

/* The timestamp immediately before the last tick() proc'd */
static nanosecond_t last_tick = NEVER;

/* 1e7 = 100 ticks per second */
const nanosecond_t Engine__tick_rate = NANOSECOND_C(10000000);

/* All logic happens when this procs */
static void tick();

/* Defined in engine.h (visible outside) */
void Engine__maybe_tick() {
	nanosecond_t crnt = time_now();

	/* Always proc the first time. */
	if (last_tick == NEVER) {
		goto proc;
	}

	/* Don't proc if it hasn't been a full cycle yet. */
	if ((crnt - last_tick) < Engine__tick_rate) {
		return;
	}

	/*
	 *If it's been more than once cycle since the last proc, we still
	 * only proc once;  this way the tick rate's lower bound is tied to
	 * the framerate.
	 *
	 * Always record the time immediately *before* the last proc, so we
	 * measure start-to-start elapsed time, not end-to-start.
	 */

proc:
	tick();
	last_tick = crnt;
}

void Engine__register_entity(Entity_t *entity) {
	add_entity(entity);
}

/* FIXME: 100% of physics are currently encapsulated in this single function */
static void rotate(Entity_t *e) {
	/* every tick (i.e. 100 times a second) rotate around y by 1 degree, and z by about a third of that */
	e->rot.y += 1.0f;
	e->rot.z += (1.0f / 2.77f);
}

/*
 * Only ever called from Engine__maybe_tick().
 * It's safe to assume it's been 'Engine__tick_rate' since last tick()
 */
static void tick() {
	each_entity(rotate);
}

/* vim: ts=4 sts=4 sw=4
 */
