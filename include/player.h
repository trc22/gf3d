#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf3d_entity.h"
#include "gfc_matrix.h"

Entity * player_spawn(Vector3D position);

void player_camera_fps(Entity* ent);








#endif