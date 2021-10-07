#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "inventory.h"
#include "interactable.h"

Matrix4 start;

int input_timer = 250;

void player_update(Entity *ent);

Entity * player_spawn()
{
    Entity *ent = gf3d_entity_create("dino");

    ent->update = player_update;
    ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);
    slog("%f, %f, %f", ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);

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


    ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);


        if(keys[SDL_SCANCODE_W])
        {
            gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * 0.075, ent->modelMat[0][0] * -0.075, 0)));
                slog("%f, %f, %f", ent->position.x,ent->position.y, ent->position.z);
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
    Matrix4 mat;
    gf3d_vgraphics_get_camera(mat);
    mat[3][0] = ent->position.x;
    mat[3][1] = ent->position.z;
    mat[3][2] = ent->position.y;
    gf3d_vgraphics_modify_camera(mat);

    /*if(ent->camera_mode == 1)
    {
        slog("Returning to start camera");
        gfc_matrix_translate(mat, vector3d(0, -1, -25));
        ent->camera_mode = 0;
        return;
    }
    gfc_matrix_translate(mat, vector3d(0, 0, 25));
    gf3d_camera_set_view(mat);
    //gf3d_camera_look_at(gf3d_vgraphics_get_camera_position(), ent->position, vector3d(0, 0, 1));
    ent->camera_mode = 1;*/
}
