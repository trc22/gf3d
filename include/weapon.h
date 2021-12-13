#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "inventory.h"
#include "gf3d_entity.h"

void weapon_use_pistol(Item* item, Entity* src);
void weapon_use_shotgun(Item* item, Entity* src);
void weapon_use_rifle(Item* item, Entity* src);


void weapon_check_all(BoundingBox *hitbox);



#endif // __WEAPON_H__
