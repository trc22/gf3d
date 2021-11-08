#include "gfc_matrix.h"

#include "enemy.h"

void enemy_update(Entity *ent);
void enemy_think(Entity *ent);
void enemy_touch(Entity* self, Entity* other);

void enemy_look_at_player(Entity* ent);

Entity *player;

Entity* enemy_spawn(Vector3D position)
{
    Entity *ent = gf3d_entity_create("misc");

    ent->isEnemy = 1;

    ent->think = enemy_think;

    gf3d_entity_set_bounding_box(ent, 1, 1, 5, 5);

    ent->healthmax = 10;
    ent->health = 10;

    vector3d_dup(ent->position);
    vector3d_copy(ent->position, position);

    return ent;
}
void enemy_think(Entity *ent)
{
    if(!player) return;
    if(player == NULL) return;

    enemy_look_at_player(ent);
}

void enemy_set_player(Entity* ent)
{
    player = ent;
}

void enemy_look_at_player(Entity* ent)
{
    Vector3D x;

    vector3d_sub(x, player->position, ent->position);
    vector3d_normalize(&x);

    if(ent->position.y > player->position.y)
        ent->rotation.z = x.x;
    else
        ent->rotation.z = -x.x + 3.14;

}
