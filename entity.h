/**
 * A first pass at defining an Entity object.
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

typedef struct Position3D {
	float x;
	float y;
	float z;
} Position3D_t;

typedef struct Entity {
	Position3D_t pos;
	Position3D_t rot; /* yay gimbol lock */
} Entity_t;

typedef enum EntityAttribute {
	ENTITY_POSITION,
	ENTITY_ROTATION,
} EntityAttribute_t;

/* Returns 0 on success. */
int Entity__set3f(Entity_t *e, EntityAttribute_t attr, float x, float y, float z);

#endif

/* vim: ts=4 sts=4 sw=4
 */
