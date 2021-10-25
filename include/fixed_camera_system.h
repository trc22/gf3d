#ifndef __FIXED_CAMERA_SYSTEM_H__
#define __FIXED_CAMERA_SYSTEM_H__

#include "gfc_types.h"

typedef struct FixedCamera_S
{
    Uint8 _inuse;
    int id;

    int isCurrent;

    Vector3D scale;
    Vector3D rotation;
    Vector3D position;

    void *data;

}FixedCamera;

void fixed_camera_system_init(Uint32 num_cameras);

void fixed_camera_switch(int id);

FixedCamera* fixed_camera_new();

void fixed_camera_free(FixedCamera *camera);

void fixed_camera_free_all();

void fixed_camera_system_close();


#endif
