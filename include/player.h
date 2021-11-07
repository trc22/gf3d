#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf3d_entity.h"
#include "gfc_matrix.h"

#include "inventory.h"

Entity * player_spawn(Vector3D position);

void player_camera_fps(Entity *ent);

void player_set_current_item(int pos);

void player_use_item(Item *item);






#endif
