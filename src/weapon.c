#include <math.h>
#include "simple_logger.h"
#include "gfc_vector.h"

#include "weapon.h"

void weapon_use_pistol(Item* item, Entity* src)
{
    Vector2D ray_start;
    Vector2D ray_end;

    if(!item) return;
    if(!src) return;



    ray_start = vector2d(src->position.x, src->position.y);
    slog("%f", src->rotation.z);
    ray_end = vector2d_rotate(vector2d(src->position.x, src->position.y - 100), src->rotation.z);


    gf3d_entity_weapon_check_all(ray_start, ray_end);

}
void weapon_use_shotgun(Item* item, Entity* src)
{

    Vector2D ray_start;
    Vector2D ray_end;

    if(!item) return;
    if(!src) return;

    ray_start = vector2d(src->position.x, src->position.y);
    slog("%f", src->rotation.z);
    ray_end = vector2d_rotate(vector2d(src->position.x, src->position.y - 100), src->rotation.z);

    gf3d_entity_weapon_check_all(ray_start, ray_end);

    ray_start = vector2d(src->position.x + 10, src->position.y);
    ray_end = vector2d_rotate(vector2d(src->position.x + 10, src->position.y - 100), src->rotation.z);

    gf3d_entity_weapon_check_all(ray_start, ray_end);

    ray_start = vector2d(src->position.x - 10, src->position.y);
    ray_end = vector2d_rotate(vector2d(src->position.x - 10, src->position.y - 100), src->rotation.z);

    gf3d_entity_weapon_check_all(ray_start, ray_end);


}

void weapon_use_rifle(Item* item, Entity* src)
{
    Vector2D ray_start;
    Vector2D ray_end;

    if(!item) return;
    if(!src) return;



    ray_start = vector2d(src->position.x, src->position.y);
    slog("%f", src->rotation.z);
    ray_end = vector2d_rotate(vector2d(src->position.x, src->position.y - 100), src->rotation.z);


    gf3d_entity_weapon_check_all_multiple(ray_start, ray_end);
}
