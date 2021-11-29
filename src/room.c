#include <stdlib.h>
#include <string.h>
#include "simple_json.h"
#include "simple_logger.h"

#include "gf3d_entity.h"
#include "player.h"
#include "room.h"

Room *room;

void room_new()
{
    room = (Room*)gfc_allocate_array(sizeof(Room),1);
    if(!room)
    {
        slog("ERROR: Cannot allocate memory for room");
    }
    room->currentCamera = (Camera*)gfc_allocate_array(sizeof(Camera),1);
}

void room_load(char* room_name)
{
    TextLine assetname;

    SJson* json, *sj_room, *sj_model;
    const char *model_name;

    snprintf(assetname,GFCLINELEN,"rooms/%s.json",room_name);
    slog("%s", assetname);

    room_new();


    if(!room)
    {
        slog("Room loading failed");
        return;
    }

    json = sj_load(assetname);

    if(!json) return;

    room->name = strdup(room_name);

    sj_room = sj_object_get_value(json, room->name);

    slog("%s", room->name);

    gfc_matrix_identity(room->modelMat);

    sj_model = sj_object_get_value(sj_room, "model");
    model_name = sj_get_string_value(sj_model);
    slog("%s", model_name);
    room->model = gf3d_model_load(strdup(model_name));

    vector3d_dup(room->position);

    room->scale = vector3d(5, 10, 5);
    room->rotation = vector3d(0, 0, 0);
    room->position = vector3d(1, 1, -12);


    sj_free(json);
/*
    gfc_matrix_scale(room->mat, vector3d(5, 10, 5));
    gfc_matrix_rotate(room->mat, room->mat, 0, vector3d(0, 0, 1));
    room->mat[3][2] = -12;

    room->extents = vector4d(20, -20, 5, -45);*/
}

void room_draw()
{
    gf3d_model_draw(room->model, room->modelMat);
}

void room_free()
{
    if(room != NULL) return;

    memset(room,0,sizeof(Room));
}

void room_set_camera(Vector3D pos, Vector3D rot)
{
    vector3d_copy(room->currentCamera->position, pos);
    vector3d_copy(room->currentCamera->rotation, rot);
    slog("current camera rotation.z = %f", room->currentCamera->rotation.z);
}
void room_camera_enable()
{
    gf3d_camera_set_position(room->currentCamera->position);
    gf3d_camera_set_rotation(room->currentCamera->rotation);
}

void room_change(char* room_name)
{
    room_free();
    gf3d_entity_free_all();
    room_load(room_name);
    /*room->mat[3][2] = -5;
    player_spawn(vector3d(0, 0, 0));
    room_set_camera(vector3d(1, 10, 1),vector3d(3.3, 0, 3.14));*/
}

int room_check_bounds(Vector3D position)
{
    if(!room) return 0;

    if(position.x >= room->extents.x) return 1;
    if(position.x <= room->extents.y) return 1;
    if(position.y >= room->extents.z) return 1;
    if(position.y <= room->extents.w) return 1;

    return 0;
}

void room_update()
{
    if (!room)return;

    gfc_matrix_identity(room->modelMat);
    gfc_matrix_scale(room->modelMat, room->scale);

    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.z, vector3d(0, 0, 1));
    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.y, vector3d(0, 1, 0));
    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.x, vector3d(1, 0, 0));

    gfc_matrix_translate(room->modelMat, room->position);

}
