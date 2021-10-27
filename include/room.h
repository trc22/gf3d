#ifndef __ROOM_H__
#define __ROOM_H__

#include "gfc_matrix.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"

typedef struct Room_S
{
    Model *model;

    Matrix4 mat;

    char *name;

    Camera* currentCamera;

    void *data;
}Room;

void room_init();

void room_new();

void room_load(char* room_name);

void room_draw(VkCommandBuffer commandBuffer, Uint32 bufferFrame);

void room_free();

void room_set_camera(Vector3D pos, Vector3D rot);

void room_camera_enable();

#endif
