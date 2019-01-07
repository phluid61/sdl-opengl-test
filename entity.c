#include "entity.h"

int Entity__set3f(Entity_t *e, EntityAttribute_t attr, float x, float y, float z) {
	switch (attr) {
	case ENTITY_POSITION:
		e->pos.x = x;
		e->pos.y = y;
		e->pos.z = z;
		break;
	case ENTITY_ROTATION:
		e->rot.x = x;
		e->rot.y = y;
		e->rot.z = z;
		break;
	default:
		return -1;
	}

	return 0;
}

/* vim: ts=4 sts=4 sw=4
 */
