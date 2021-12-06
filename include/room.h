#ifndef __ROOM_H__
#define __ROOM_H__

#include "gfc_matrix.h"
#include "simple_json.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"

typedef struct Room_S
{
    Model *model;

    Matrix4 modelMat;

    Vector3D position;
    Vector3D rotation;
    Vector3D scale;

    Vector4D extents;

    char *name;

    Camera* currentCamera;

    void *data;
}Room;

void room_init();

void room_new();

void room_load(char* room_name);

void room_load_entities(SJson *sj_room);

void room_draw();

void room_free();

void room_set_camera(Vector3D pos, Vector3D rot);

void room_camera_enable();

void room_change(char* room_name);

int room_check_bounds(Vector3D position);

void room_update();

#endif
