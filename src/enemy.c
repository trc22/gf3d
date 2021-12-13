#include "gfc_matrix.h"

#include "enemy.h"
#include "room.h"


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

Entity* enemy_spawn_boss(Vector3D position)
{
    Entity *ent = gf3d_entity_create("misc");

    ent->isEnemy = 2;
    ent->misc = 3;

    ent->think = enemy_think;

    gf3d_entity_set_bounding_box(ent, -1, -1, 5, 5);

    ent->healthmax = 10;
    ent->health = 10;

    vector3d_dup(ent->position);
    vector3d_copy(ent->position, position);

    vector3d_copy(ent->scale, vector3d(2, 2, 2));


    return ent;
}

void enemy_think(Entity *ent)
{
    if(!player) return;
    if(player == NULL) return;

    enemy_look_at_player(ent);
    if(ent->isEnemy == 2)
    {
        if(!vector3d_distance_between_less_than(ent->position, player->position, 12))
        {
            ent->position.x += 0.01 * (sin(ent->rotation.z));
            ent->position.y -= 0.01 * (cos(ent->rotation.z));

        }
        return;
    }
    if(!vector3d_distance_between_less_than(ent->position, player->position, 8))
    {
        ent->position.x += 0.01 * (sin(ent->rotation.z));
        ent->position.y -= 0.01 * (cos(ent->rotation.z));

    }

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
        ent->rotation.z = -x.x - 3.14;

}
