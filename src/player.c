#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "inventory.h"
#include "interactable.h"
#include "room.h"

#include <math.h>
Matrix4 start;

int input_timer = 250;

void player_update(Entity *ent);
void player_think(Entity *ent);
void player_touch(Entity* self, Entity* other);


Entity * player_spawn(Vector3D position)
{
    Entity *ent = gf3d_entity_create("misc");

    ent->update = player_update;
    ent->think = player_think;
    ent->touch = player_touch;

    ent->camera_mode = 0;

    inventory_init(8);

    inventory_load_item("test item");
    inventory_load_item("pistol");

    gf3d_entity_set_bounding_box(ent, vector3d(-1, -1, 0), vector3d(1, 1, 0));

    slog("Bounding box for player: %f to %f", ent->boundingBox->minExtent.x, ent->boundingBox->maxExtent.x);

    vector3d_dup(ent->position);

    return ent;
}

void player_think(Entity *ent)
{
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);

        if(ent->camera_mode == 0)
        {
            if(keys[SDL_SCANCODE_W])
            {
                //gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * 0.075, ent->modelMat[0][0] * -0.075, 0)));    ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);
                ent->position.x += 0.1 * (sin(ent->rotation.z));
                ent->position.y -= 0.1 * (cos(ent->rotation.z));
                bounding_box_update(ent->boundingBox, ent->position);
                //slog("%f, %f, %f", ent->boundingBox->minExtentPos.x, ent->boundingBox->minExtentPos.y, ent->boundingBox->minExtentPos.z);
            }
            if(keys[SDL_SCANCODE_S])
            {
                ent->position.x -= 0.1 * (sin(ent->rotation.z));
                ent->position.y += 0.1 * (cos(ent->rotation.z));
                bounding_box_update(ent->boundingBox, ent->position);
            }
        }
        if(keys[SDL_SCANCODE_A])
        {
            if(ent->camera_mode == 1)
            {
                ent->rotation.z += 0.0025;
                gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));
            }
            else
                ent->rotation.z += 0.0075;

        }
        if(keys[SDL_SCANCODE_D])
        {
            if(ent->camera_mode == 1)
            {
                ent->rotation.z -= 0.0025;
                gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));
            }
            else
                ent->rotation.z -= 0.0075;
        }


        if(input_timer == 250)
        {   if(keys[SDL_SCANCODE_LSHIFT])
            {
                player_camera_fps(ent);

                input_timer = 0;
            }
            if(keys[SDL_SCANCODE_E])
            {
                if(ent->interactable)
                {
                    interactable_interact(ent->interactable);
                    ent->interactable = NULL;
                }
            }

        }
        else
            input_timer += 1;

}

void player_update(Entity* ent)
{

}

void player_camera_fps(Entity* ent)
{

    if(ent->camera_mode == 1)
    {
        ent->camera_mode = 0;
        room_camera_enable();
        return;
    }

    gf3d_camera_set_position(ent->position);
    gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));

    ent->camera_mode = 1;
}

void player_touch(Entity* self, Entity* other)
{
    if(!self || !other)
        return;
    if(other->interactable)
        self->interactable = other->interactable;
    else
        self->interactable = NULL;
}
