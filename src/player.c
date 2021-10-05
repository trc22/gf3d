#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "inventory.h"

Matrix4 start;

int input_timer = 100;

void player_update(Entity *ent);

Entity * player_spawn()
{
    Entity *ent = gf3d_entity_create("misc");

    ent->update = player_update;

    ent->camera_mode = 0;

    inventory_init(8);

    inventory_load_item("test item");
    inventory_load_item("pistol");

    return ent;
}

void player_update(Entity *ent)
{
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);


    ent->position = vector3d(ent->modelMat[0][3],ent->modelMat[1][3], ent->modelMat[2][3]);


        if(keys[SDL_SCANCODE_W])
        {
            gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * 0.075, ent->modelMat[0][0] * -0.075, 0)));
        }
        if(keys[SDL_SCANCODE_S])
            gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * -0.075, ent->modelMat[0][0] * 0.075, 0)));
        if(keys[SDL_SCANCODE_A])
            gfc_matrix_rotate(
                ent->modelMat,
                ent->modelMat,
                0.0075,
                vector3d(0,0,1));
        if(keys[SDL_SCANCODE_D])
            gfc_matrix_rotate(
                ent->modelMat,
                ent->modelMat,
                -0.0075,
                vector3d(0,0,1));

        if(input_timer == 100)
        {   if(keys[SDL_SCANCODE_LSHIFT])
            {
                if(ent->camera_mode == 0)
                    player_camera_fps(ent);
                else
                {
                    slog("Returning to start camera");
                    //gf3d_camera_get_view(start);
                    ent->camera_mode = 0;
                }

                input_timer = 0;
            }
        }
        else
            input_timer += 1;

        if(keys[SDL_SCANCODE_SPACE])
        {
            gf3d_camera_move(vector3d(1, 0, 0));
        }
}

void player_camera_fps(Entity* ent)
{
    //gfc_matrix_slog(ent->modelMat);
    slog("Switching to fps");
    gf3d_camera_set_view(ent->modelMat);
    ent->camera_mode = 1;
}
