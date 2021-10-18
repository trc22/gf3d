#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "inventory.h"
#include "interactable.h"

#include <math.h>
Matrix4 start;

int input_timer = 250;

void player_update(Entity *ent);
void player_think(Entity *ent);

Entity * player_spawn(Vector3D position)
{
    Entity *ent = gf3d_entity_create("dino");

    ent->update = player_update;
    //ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);
    slog("%f, %f, %f", ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);

    ent->camera_mode = 0;

    inventory_init(8);

    inventory_load_item("test item");
    inventory_load_item("pistol");

    vector3d_dup(ent->position);

    return ent;
}

void player_update(Entity *ent)
{
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);


        if(keys[SDL_SCANCODE_W])
        {
            //gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * 0.075, ent->modelMat[0][0] * -0.075, 0)));    ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);
            ent->position.x += 0.1 * (sin(ent->rotation.z));
            ent->position.y -= 0.1 * (cos(ent->rotation.z));
                //slog("%f, %f, %f", ent->position.x,ent->position.y, ent->position.z);
        }
        if(keys[SDL_SCANCODE_S])
        {
            ent->position.x -= 0.1 * (sin(ent->rotation.z));
            ent->position.y += 0.1 * (cos(ent->rotation.z));
        }

        if(keys[SDL_SCANCODE_A])
            ent->rotation.z += 0.0075;
        if(keys[SDL_SCANCODE_D])
            ent->rotation.z -= 0.0075;

        if(input_timer == 250)
        {   if(keys[SDL_SCANCODE_LSHIFT])
            {
                player_camera_fps(ent);

                input_timer = 0;
            }
        }
        else
            input_timer += 1;
}

void player_camera_fps(Entity* ent)
{

    if(ent->camera_mode == 1)
    {
        slog("Returning to start camera");
        ent->camera_mode = 0;
        return;
    }

    gf3d_camera_set_rotation(vector3d(1.57, 0, ent->rotation.z + 3.14));
    gf3d_camera_set_position(vector3d(ent->position.x, 2, ent->position.y));

    ent->camera_mode = 1;
}
