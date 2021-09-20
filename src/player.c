#include "gf3d_entity.h"
#include "gfc_matrix.h"

void player_update(Entity *ent);

Entity * player_spawn()
{
    Entity *ent = gf3d_entity_new();
    ent->model = gf3d_model_load("misc");
    gfc_matrix_identity(ent->modelMat);

    ent->update = player_update;
    return ent;
}

void player_update(Entity *ent)
{
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);
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
}
