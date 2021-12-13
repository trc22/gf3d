#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "gf3d_entity.h"

Entity* enemy_spawn(Vector3D position);

Entity* enemy_spawn_boss(Vector3D position);

void enemy_set_player(Entity* ent);

#endif // __ENEMY_H__
